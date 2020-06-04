#ifndef coln_proto_map_h
#define coln_proto_map_h
#include "google/protobuf/map.h"
#include "util/includes.h"

namespace vectorbook {

namespace proto_map {
template <typename K, typename V>
class ConstUnorderedMapIterator : public ConstIterator<KV<K, V>> {
public:
  ConstUnorderedMapIterator(const google::protobuf::Map<K, V> *underlying) {
    curr_ = underlying->begin();
    end_ = underlying->end();
  }
  Const<KV<K, V>> current() const override {
    auto s = new KV<K, V>(curr_->first, curr_->second);
    return s;
  }
  bool done() { return curr_ == end_; }
  void advance() { ++curr_; }

private:
  using ProtoMapType = google::protobuf::Map<K, V>;
  typename ProtoMapType::const_iterator curr_;
  typename ProtoMapType::const_iterator end_;
};
} // namespace proto_map

// A Map backed by a protocol buffer.
template <typename K, typename V> class ProtoMap : public Map<K, V> {
public:
  using ProtoMapType = google::protobuf::Map<K, V>;
  ProtoMap(ProtoMapType *in) : under_(in) {}

  ProtoMap(ProtoMapType *in, const std::function<V()> &factory_fn)
      : under_(in), factory_fn_(factory_fn) {}
  ProtoMap(const ProtoMap<K, V> &other) = default;
  ProtoMap<K, V> &operator=(const ProtoMap<K, V> &other) = default;
  ProtoMap(ProtoMap<K, V> &&other) = default;
  ProtoMap<K, V> &operator=(ProtoMap<K, V> &&other) = default;
  Size size() const override { return CopyUnique(under_->size()); }
  Boolean contains(const K &k) const { RAISE_EXCEPTION("tbd"); }
  Void put(const K &k, const V &v) override {
    (*under_)[k] = v;
    return Ok();
  }
  Void put(const K &k, V &&v) override {
    (*under_)[k] = std::move(v);
    return Ok();
  }
  Const<V> const_get(const K &k) const override {
    auto ptr = under_->find(k);
    if (ptr != under_->end()) {
      return Const<V>(&(ptr->second));
    } else {
      return Null();
    }
  }
  Mutable<V> mutable_get(const K &k) override {
    auto ptr = under_->find(k);
    if (ptr != under_->end()) {
      return Mutable<V>(&(ptr->second));
    } else {
      if (factory_fn_.has_value()) {
        ASSERT(factory_fn_.has_value());
        (*under_)[k] = (*factory_fn_)();
        auto ptr = under_->find(k);
        return Mutable<V>(&(ptr->second));
      } else {
        return Null();
      }
    }
  }

  Shared<ConstIterator<KV<K, V>>> iterator() const override {
    Shared<ConstIterator<KV<K, V>>> vi(
        new proto_map::ConstUnorderedMapIterator<K, V>(under_));
    return vi;
  }

  Void erase(const K &k) override {
    under_->erase(k);
    return Ok();
  }

private:
  ProtoMapType *under_;
  absl::optional<std::function<V()>> factory_fn_;
};

// A map backed by a RepeatedPtrField.
//
// REQUIRES: The key must be called 'name' and the value must be called
// 'value'.
template <typename ITEM, typename K, typename V>
class RepeatedPtrMapBase : public Map<K, V> {
protected:
  virtual void SetValue(V v, ITEM *dest) const = 0;

public:
  RepeatedPtrMapBase(RepeatedPtrField<ITEM> *in) : under_(in) {}

  RepeatedPtrMapBase(RepeatedPtrField<ITEM> *in,
                     const std::function<V()> &factory_fn)
      : under_(in), factory_fn_(factory_fn) {}
  RepeatedPtrMapBase(const RepeatedPtrMapBase<ITEM, K, V> &other) = default;
  RepeatedPtrMapBase<ITEM, K, V> &
  operator=(const RepeatedPtrMapBase<ITEM, K, V> &other) = default;
  RepeatedPtrMapBase(RepeatedPtrMapBase<ITEM, K, V> &&other) = default;
  RepeatedPtrMapBase<ITEM, K, V> &
  operator=(RepeatedPtrMapBase<ITEM, K, V> &&other) = default;

  Size size() const override { return under_->size(); }
  Boolean contains(const K &k) const { RAISE_EXCEPTION("tbd"); }
  Void put(const K &k, const V &v) override {
    for (ITEM &item : *under_) {
      if (item.name() == k) {
        // item.set_value(v);
				SetValue(v, &item);
        return Ok();
      }
    }
    // Not found. Add to end.
    auto new_label = under_->Add();
    new_label->set_name(k);
		SetValue(v, new_label);
    // new_label->set_value(v);
    return Ok();
  }
  Void put(const K &k, V &&v) override { return put(k, v); }
  Const<V> const_get(const K &k) const override {
    for (ITEM &item : *under_) {
      if (item.name() == k)
        return CopyUnique(item.value());
    }
    return Null();
  }
  Mutable<V> mutable_get(const K &k) override {
    auto value = const_get(k);
    ASSERT_OK(value);
    if (!value.has_value()) {
      if (factory_fn_.has_value()) {
        auto v = (*factory_fn_)();
        auto status = put(k, v);
        ASSERT_OK(status);
        value = const_get(k);
        ASSERT_OK(value);
      } else {
        RAISE_EXCEPTION("No value found, and no factory.");
      }
    }
    auto write_back = [k, this](const V &v) { return this->put(k, v); };
    auto kp = new V(*value);
    std::unique_ptr<V> up(kp);
    return Mutable<V>(write_back, std::move(up));
  }
  Shared<ConstIterator<KV<K, V>>> iterator() const override { RAISE_EXCEPTION("TBD"); }
  Void erase(const K &k) override { RAISE_EXCEPTION("TBD"); }

private:
  RepeatedPtrField<ITEM> *under_;
  absl::optional<std::function<V()>> factory_fn_;
};

template <typename ITEM, typename K, typename V>
class RepeatedEnumMap : public RepeatedPtrMapBase<ITEM, K, V> {
protected:
  virtual void SetValue(V v, ITEM *dest) const override { dest->set_value(v); }

public:
  RepeatedEnumMap(RepeatedPtrField<ITEM> *in)
      : RepeatedPtrMapBase<ITEM, K, V>(in) {}

  RepeatedEnumMap(RepeatedPtrField<ITEM> *in,
                  const std::function<V()> &factory_fn)
      : RepeatedPtrMapBase<ITEM, K, V>(in, factory_fn) {}
};

template <typename ITEM, typename K, typename V>
class RepeatedProtoMap : public RepeatedPtrMapBase<ITEM, K, V> {
protected:
  virtual void SetValue(V v, ITEM *dest) const override {
    *dest->mutable_value() = v;
  }

public:
  RepeatedProtoMap(RepeatedPtrField<ITEM> *in)
      : RepeatedPtrMapBase<ITEM, K, V>(in) {}

  RepeatedProtoMap(RepeatedPtrField<ITEM> *in,
                   const std::function<V()> &factory_fn)
      : RepeatedPtrMapBase<ITEM, K, V>(in, factory_fn) {}
};

} // namespace vectorbook
#endif /* coln_proto_map_h */
