#include "data/set.h"

#include "util/includes.h"

namespace vectorbook {
namespace data {

unique_ptr<Set<string>> RedisSet(const string &key) {
  return unique_ptr<Set<string>>(new impl::RedisSet(key));
}

Option<string> RedisRandomSetElement(const string& set_key) {
	RedisConnection *connection = StaticRedisConnection();
	auto result = connection->Execute({"SRANDMEMBER %s", set_key});
	ASSERT_OK(result);
	return result->string_value();
}

} // namespace data
} // namespace vectorbook
