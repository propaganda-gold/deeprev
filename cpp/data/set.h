#ifndef data_set_h
#define data_set_h 
#include "coln/api.h"
#include "coln/set.h"
#include "data/connection.h"
#include "util/includes.h"

namespace vectorbook {
namespace data {
namespace impl {
// TODO(greg) This is the same as RedisSeqIterator.
class RedisSetIterator : public ConstIterator<string> {
public:
  RedisSetIterator(RedisResponse &&underlying)
      : underlying_(std::move(underlying)) {}
  Const<string> current() const override {
		return Const<string>(new string(underlying_.child(idx_).string_value()));
  }
  bool done() { 
		return idx_ == underlying_.child_size();
	}
  void advance() { ++idx_; }

private:
  RedisResponse underlying_;
  int idx_ = 0;
};

class RedisSet : public Set<string> {
public:
  RedisSet(const string &key) : key_(key) {}
  Void erase(const string &k) override {
		RAISE_EXCEPTION("not implemented");
	}
  Boolean contains(const string &k) const override {
    RedisConnection *connection = StaticRedisConnection();
    auto response = connection->Execute({"SISMEMBER %s %s", key_, k});
		ASSERT_OK(response);
    return response->bool_value();
	}
	Size size() const override {
    RedisConnection *connection = StaticRedisConnection();
    RedisResponse redis_response;
    auto response = connection->Execute({"SCARD %s", key_});
		ASSERT_OK(response);
    return response->int_value();
	}
	Void insert(const string&value) override {
		RedisConnection *connection = StaticRedisConnection();
		RedisResponse redis_response;
		auto status = connection->Execute({"SADD %s %s", key_, value});
		ASSERT_OK(status);
		return Ok();
	}
	Void insert(string&&t) override {
		auto status = insert(t);
		ASSERT_OK(status);
		return status;
	}
	Shared<ConstIterator<string>> iterator() const override {
    RedisConnection *connection = StaticRedisConnection();
    RedisResponse redis_response;
    auto response = connection->Execute({"SMEMBERS %s", key_});
		return new RedisSetIterator(std::move(*response));
	}
private:
  string key_;
  unordered_map<string, string> under_;
};

} // namespace impl

unique_ptr<Set<string>> RedisSet(const string &key) ;

Option<string> RedisRandomSetElement(const string& set_key);

} // namespace data
} // namespace vectorbook
#endif /* data_set_h */
