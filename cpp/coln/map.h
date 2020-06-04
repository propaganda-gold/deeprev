#ifndef coln_map_h
#define coln_map_h
#include "absl/types/optional.h"
#include "coln/api.h"
#include "coln/option.h"

namespace vectorbook {

template <typename K, typename V>
class ConstUnorderedMapIterator : public ConstIterator<KV<K, V>> {
public:
  ConstUnorderedMapIterator(const std::unordered_map<K, V> *underlying) {
    curr_ = underlying->begin();
    end_ = underlying->end();
  }
  Const<KV<K, V>> current() const override {
    auto s = new KV<K, V>(&*curr_);
    return s;
  }
  bool done() { return curr_ == end_; }
  void advance() { ++curr_; }

private:
  typename std::unordered_map<K, V>::const_iterator curr_;
  typename std::unordered_map<K, V>::const_iterator end_;
};

template <typename K, typename V>
class UnorderedMapIterator : public MutableIterator<KV<K, V>> {
public:
  UnorderedMapIterator(std::unordered_map<K, V> *underlying) {
    curr_ = underlying->begin();
    end_ = underlying->end();
  }
  KV<K, V> &current() override { return *curr_; }
  bool done() { return curr_ == end_; }
  void advance() { ++curr_; }

private:
  typename std::unordered_map<K, V>::iterator curr_;
  typename std::unordered_map<K, V>::iterator end_;
};

template <typename K, typename V> class StdMap : public Map<K, V> {
public:
  StdMap() {}
  StdMap(std::unordered_map<K, V> &&in) : under_(std::move(in)) {}
  StdMap(const std::function<V()> &factory_fn) : factory_fn_(factory_fn) {}
  StdMap(const StdMap<K, V> &other) = default;
  StdMap<K, V> &operator=(const StdMap<K, V> &other) = default;
  StdMap(StdMap<K, V> &&other) = default;
  StdMap<K, V> &operator=(StdMap<K, V> &&other) = default;
  Size size() const override { return under_.size(); }
  Boolean contains(const K &k) const { RAISE_EXCEPTION("tbd"); }
  Void put(const K &k, const V &v) override {
    under_[k] = v;
    return Ok();
  }
  Void put(const K &k, V &&v) override {
    under_[k] = std::move(v);
    return Ok();
  }
  Const<V> const_get(const K &k) const override {
    auto ptr = under_.find(k);
    if (ptr != under_.end()) {
      return Const<V>(&(ptr->second));
    } else {
      return Null();
    }
  }
  Mutable<V> mutable_get(const K &k) override {
    auto ptr = under_.find(k);
    if (ptr != under_.end()) {
      return Mutable<V>(&(ptr->second));
    } else {
      if (factory_fn_.has_value()) {
        ASSERT(factory_fn_.has_value());
        under_[k] = (*factory_fn_)();
        auto ptr = under_.find(k);
        return Mutable<V>(&(ptr->second));
      } else {
        return Null();
      }
    }
  }

  Shared<ConstIterator<KV<K, V>>> iterator() const override {
    Shared<ConstIterator<KV<K, V>>> vi(
        new ConstUnorderedMapIterator<K, V>(&under_));
    return vi;
  }

  Void erase(const K &k) override {
    under_.erase(k);
    return Ok();
  }

private:
  std::unordered_map<K, V> under_;
  absl::optional<std::function<V()>> factory_fn_;
};

// TODO(greg) Eliminate this class using traits.
template <typename K, typename V> class StdUniqueMap : public Map<K, V> {
public:
  StdUniqueMap() {}
  StdUniqueMap(std::unordered_map<K, V> &&in) : under_(std::move(in)) {}
  StdUniqueMap(const std::function<V()> &factory_fn) : factory_fn_(factory_fn) {}
  StdUniqueMap(const StdUniqueMap<K, V> &other) = default;
  StdUniqueMap<K, V> &operator=(const StdUniqueMap<K, V> &other) = default;
  StdUniqueMap(StdUniqueMap<K, V> &&other) = default;
  StdUniqueMap<K, V> &operator=(StdUniqueMap<K, V> &&other) = default;
  Size size() const override { return under_.size(); }
  Boolean contains(const K &k) const { RAISE_EXCEPTION("tbd"); }
  Void put(const K &k, const V &v) override {
    RAISE_EXCEPTION("Not implemented");
  }
  Void put(const K &k, V &&v) override {
    under_[k] = std::move(v);
    return Ok();
  }
  Const<V> const_get(const K &k) const override {
    auto ptr = under_.find(k);
    if (ptr != under_.end()) {
      return Const<V>(&(ptr->second));
    } else {
      return Null();
    }
  }
  Mutable<V> mutable_get(const K &k) override {
    auto ptr = under_.find(k);
    if (ptr != under_.end()) {
      return Mutable<V>(&(ptr->second));
    } else {
      if (factory_fn_.has_value()) {
        ASSERT(factory_fn_.has_value());
        under_[k] = (*factory_fn_)();
        auto ptr = under_.find(k);
        return Mutable<V>(&(ptr->second));
      } else {
        return Null();
      }
    }
  }

  Shared<ConstIterator<KV<K, V>>> iterator() const override {
    Shared<ConstIterator<KV<K, V>>> vi(
        new ConstUnorderedMapIterator<K, V>(&under_));
    return vi;
  }

  Void erase(const K &k) override {
    under_.erase(k);
    return Ok();
  }

private:
  std::unordered_map<K, V> under_;
  absl::optional<std::function<V()>> factory_fn_;
};

// The default Map, which is backed by an unordered_map.
template <typename K, typename V> Unique<Map<K, V>> MakeMap() {
  return Unique<Map<K, V>>(new StdMap<K, V>());
}

std::function<int()> EmptyInt();
std::function<float()> EmptyFloat();
template <typename T> std::function<T()> EmptyProto() {
  return []() { return T(); };
}


template <typename K, typename V>
std::function<StdMap<K, V>()> EmptyStdMap(std::function<V()> inner_start_fn) {
  return [inner_start_fn]() { return StdMap<K, V>(inner_start_fn); };
}

template <typename K, typename V>
Unique<Map<K, V>> MakeMap(const std::function<V()> &start_fn) {
  return Unique<Map<K, V>>(new StdMap<K, V>(start_fn));
}

using StringPair = std::pair<std::string, std::string>;

} // namespace vectorbook
#endif /* coln_map_h */
