#include "data/map.h"

#include "util/includes.h"

namespace vectorbook {
namespace data {
namespace impl {
Const<string> GetRedisMapValue(const string &key, const string &k) {
  RedisConnection *connection = StaticRedisConnection();
  auto redis_response = connection->Execute({"HGET %s %s", key, k});
	ASSERT_OK(redis_response);
  if (redis_response->has_string_value()) {
    return new string(redis_response->string_value());
  } else {
    return Null();
  }
}

} // namespace impl

unique_ptr<Map<string, string>> RedisStringMap(const string &key) {
  return unique_ptr<Map<string, string>>(new impl::RedisStringMap(key));
}

} // namespace data
} // namespace vectorbook
