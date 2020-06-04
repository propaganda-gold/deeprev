#include "data/test_only.h"

#include "data/connection.h"
#include "util/includes.h"

namespace vectorbook {

Void ClearLocalDatabase() {
	RedisConnection *connection = StaticRedisConnection();
	auto status = connection->Execute({"FLUSHALL"});
	ASSERT_OK(status);
	return Ok();
}

} // namespace vectorbook
