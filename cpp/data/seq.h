#ifndef data_seq_h
#define data_seq_h
#include "coln/api.h"
#include "coln/seq.h"
#include "data/connection.h"
#include "data/converter.h"
#include "util/includes.h"

namespace vectorbook {
namespace data {
namespace impl {
class StringQueueIterator : public ConstIterator<string> {
public:
  StringQueueIterator(int direction, RedisResponse &&under)
      : direction_(direction), under_(std::move(under)) {
    if (direction > 0) {
      idx_ = 0;
    } else {
      idx_ = under_.child_size() - 1;
    }
  }
  Const<string> current() const override {
    return Const<string>(new string(under_.child(idx_).string_value()));
  }
  bool done() {
    if (direction_ > 0) {
      return idx_ == under_.child_size();
    } else {
      return idx_ < 0;
    }
  }
  void advance() { idx_ += direction_; }

private:
  RedisResponse under_;
  int idx_ = 0;
  int direction_;
};

// TODO(greg) Move to cc part.
class RedisStringQueue : public Queue<string> {
public:
  RedisStringQueue(const string &key) : key_(key) {}
  Size size() const override {
    RedisConnection *connection = StaticRedisConnection();
    auto redis_response = connection->Execute({"LLEN %s", key_});
    ASSERT_OK(redis_response);
    return redis_response->int_value();
  }
  Void append(const string &value) override {
    RedisConnection *connection = StaticRedisConnection();
    auto response = connection->Execute({"RPUSH %s %s", key_, value});
    ASSERT_OK(response);
    return Ok();
  }
  Void append(string &&t) override {
    return append(t);
  }
  Option<string> pop() override {
    RedisConnection *connection = StaticRedisConnection();
    auto response = connection->Execute({"LPOP %s", key_});
    ASSERT_OK(response);
		DEBUG_ASSERT(response.has_value());
		if (response->error_code() == 4) {
			return Null();
		} else {
			DEBUG_ASSERT(response->error_code() == 1);
			return response->string_value();
		}
	}
	Shared<ConstIterator<string>> iterator() const override {
    RedisConnection *connection = StaticRedisConnection();
    auto redis_response= connection->Execute({"LRANGE %s 0 -1", key_});
		ASSERT_OK(redis_response);
    Shared<ConstIterator<string>> vi(
        new StringQueueIterator(1, std::move(redis_response.expose())));
    return vi;
  }

private:
  string key_;
  unordered_map<string, string> under_;
};

template <typename T> class ProtoQueueIterator : public ConstIterator<T> {
public:
  ProtoQueueIterator(const Serializer<T> *serializer,
                     unique_ptr<ConstIterator<string>> &&under)
      : serializer_(serializer), under_(std::move(under)) {}
  Const<T> current() override {
    T cache_;
    auto status = Assign(serializer_->Parse(under_->current()), &cache_);
    assert(status.ok());
    return cache_;
  }
  bool done() { return under_->done(); }
  void advance() { under_->advance(); }

private:
  const Serializer<T> *serializer_;
  unique_ptr<ConstIterator<string>> under_;
};

template <typename V> class RedisProtoQueue : public Queue<V> {
public:
  RedisProtoQueue(const string &key) : key_(key) {
    under_ = RedisStringQueue(key);
  }
  Size size() const override { return under_.size(); }
  Void append(const V &t) override {
    under_.append(serializer_->Serialize(t));
    return Ok();
  }
  Void append(string &&t) override { return append(t); }

  Shared<ConstIterator<string>> iterator() override {
		RAISE_EXCEPTION("Not implemented.");
//    auto string_it = under_.iterator();
//    return nullptr;
  }

private:
  string key_;
  RedisStringQueue under_;
  unique_ptr<Serializer<V>> serializer_;
};

} // namespace impl

unique_ptr<Queue<string>> RedisStringQueue(const string &key);

} // namespace data
} // namespace vectorbook
#endif /* data_seq_h */
