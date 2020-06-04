#ifndef data_map_h
#define data_map_h
#include "coln/api.h"
#include "coln/map.h"
#include "data/connection.h"
#include "data/converter.h"
#include "util/includes.h"

namespace vectorbook {
namespace data {
namespace impl {
Const<string> GetRedisMapValue(const string &key, const string &k);

class StringMapIterator : public ConstIterator<KV<string, string>> {
public:
  StringMapIterator(const string &key, Unique<RedisResponse> &&underlying)
      : key_(key), underlying_(std::move(underlying)) {}
  Const<KV<string, string>> current() const override {
    const string &k = underlying_->child(idx_).string_value();
    auto value_opt = GetRedisMapValue(key_, k);
    ASSERT_OK(value_opt);
    ASSERT(value_opt.has_value());
    return new KV<string, string>(k, *value_opt);
  }
  bool done() { return idx_ == underlying_->child_size(); }
  void advance() { ++idx_; }

private:
  string key_;
  Unique<RedisResponse> underlying_;
  int idx_ = 0;
};

class RedisStringMap : public StringMap {
public:
  RedisStringMap(const string &key) : key_(key) {}
  Size size() const override {
    RedisConnection *connection = StaticRedisConnection();
    auto redis_response = connection->Execute({"HLEN %s", key_});
    ASSERT_OK(redis_response);
    return redis_response->int_value();
  }
  Void erase(const string &k) override {
    RedisConnection *connection = StaticRedisConnection();
    auto redis_response = connection->Execute({"HDEL %s %s", key_, k});
    ASSERT_OK(redis_response);
    return Ok();
	}
  Boolean contains(const string &k) const {
    RedisConnection *connection = StaticRedisConnection();
    auto redis_response = connection->Execute({"HEXISTS %s %s", key_, k});
    ASSERT_OK(redis_response);
    return static_cast<bool>(redis_response->int_value());
  }
  Const<string> const_get(const string &k) const override {
    return GetRedisMapValue(key_, k);
  }
  Mutable<string> mutable_get(const string &k) {
    auto value = const_get(k);
    ASSERT_OK(value);
    string key = key_;
    auto write_back = [k, key](const string &v) {
      auto map = RedisStringMap(key);
      return map.put(k, v);
    };
    auto kp = new string(*value);
    return Mutable<string>(write_back, kp);
  }

  Void put(const string &k, const string &v) override {
    RedisConnection *connection = StaticRedisConnection();
    RedisResponse redis_response;
    auto result = connection->Execute({"HSET %s %s %s", key_, k, v});
    ASSERT_OK(result);
    return Ok();
  }
  Void put(const string &k, string &&v) override { return put(k, v); }

  Shared<ConstIterator<KV<string, string>>> iterator() const override {
    RedisConnection *connection = StaticRedisConnection();
    auto redis_response = connection->Execute({"HKEYS %s", key_});
    assert(redis_response.ok());
    return Shared<ConstIterator<KV<string, string>>>(
        new StringMapIterator(key_, std::move(redis_response)));
  }

private:
  string key_;
  unordered_map<string, string> under_;
};

template <typename T>
class ProtoMapIterator : public ConstIterator<KV<string, T>> {
public:
  ProtoMapIterator(const Serializer<T> *serializer,
                   const Shared<ConstIterator<KV<string, string>>> &under)
      : serializer_(serializer), under_(under) {}
  Const<KV<string, T>> current() const override {
    auto current = under_->current();
    ASSERT_OK(current);
    auto t = serializer_->Parse(current->value());
    ASSERT_OK(t);
    ASSERT_OK(t);
    return Unique<KV<string, T>>(new KV<string, T>(current->key(), *t));
  }
  bool done() { return under_->done(); }
  void advance() { under_->advance(); }

private:
  const Serializer<T> *serializer_;
  Shared<ConstIterator<KV<string, string>>> under_;
};

template <typename V> class RedisProtoMap : public Map<string, V> {
public:
  RedisProtoMap(const string &key) : key_(key) {
    under_.reset(new RedisStringMap(key));
    serializer_ = MakeDebugSerializer<V>();
  }
  RedisProtoMap(const string &key, const std::function<V()> &factory_fn)
      : key_(key), factory_fn_(factory_fn) {
    under_.reset(new RedisStringMap(key));
    serializer_ = MakeDebugSerializer<V>();
  }
  Size size() const override { return under_->size(); }
  Boolean contains(const string &k) const { return under_->contains(k); }
  Void erase(const string &k) { 
		auto status = under_->erase(k); ASSERT_OK(status);
		return Ok();
	}
  Const<V> const_get(const string &k) const override {
    Const<string> sopt = under_->const_get(k);
    if (!sopt.has_value()) {
      if (factory_fn_.has_value()) {
        ASSERT(factory_fn_.has_value());
        auto v = (*factory_fn_)();
        auto status = put(k, v);
        // TODO(greg) clean up.
        return new V(v);
      } else {
        return Null();
      }
    } else {
      auto r = serializer_->Parse(*sopt);
      ASSERT_OK(r);
      return Const<V>(std::move(r));
    }
  }
  Mutable<V> mutable_get(const string &k) override {
    auto value = const_get(k);
    ASSERT_EXISTS(value);
    string key = key_;
    auto write_back = [k, key](const V &v) {
      return RedisProtoMap(key).put(k, v);
    };
    auto kp = new V(*value);
    return Mutable<V>(write_back, kp);
  }
  Void put(const string &k, const V &v) override {
    auto serialized = serializer_->Serialize(v);
    ASSERT_OK(serialized);
    return under_->put(k, *serialized);
  }
  Void put(const string &k, V &&v) override { return put(k, v); }

  Shared<ConstIterator<KV<string, V>>> iterator() const override {
    auto pmi = new ProtoMapIterator<V>(serializer_.get(), under_->iterator());
    return Shared<ConstIterator<KV<string, V>>>(pmi);
  }

private:
  string key_;
  Unique<RedisStringMap> under_;
  Unique<Serializer<V>> serializer_;
  absl::optional<std::function<V()>> factory_fn_;
};

} // namespace impl

unique_ptr<Map<string, string>> RedisStringMap(const string &key);

template <typename V> unique_ptr<Map<string, V>> RedisProtoMap(const string &key) {
  return unique_ptr<Map<string, V>>(new impl::RedisProtoMap<V>(key));
}

template <typename V>
unique_ptr<Map<string, V>> RedisProtoMap(const string &key,
                                     const std::function<V()> &factory_fn) {
  return unique_ptr<Map<string, V>>(new impl::RedisProtoMap<V>(key, factory_fn));
}

} // namespace data
} // namespace vectorbook
#endif /* data_map_h */
