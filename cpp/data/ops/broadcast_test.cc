#include "data/ops/broadcast.h"

#include "data/connection.h"
#include "data/test_only.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

//TEST(BuildBroadcast, BasicText) {
//	Broadcast::BasicData data;
//	data.set_textarea("This is a basic text.");
//	auto broadcast = BuildBroadcast(data);
//	ASSERT_TRUE(broadcast.ok());
//}

TEST(BuildBroadcast, Url) {
	auto create = CreateRedisConnection();
	ASSERT_TRUE(create.ok());

	auto clear = ClearLocalDatabase();
	ASSERT_TRUE(clear.ok());

	Broadcast::BasicData data;
	data.set_textarea("check out https://www.youtube.com/watch?v=s8V6NxdmpO0");
	auto broadcast = BuildBroadcast(data);
	ASSERT_TRUE(broadcast.ok());
	ASSERT_TRUE(broadcast.has_value());
}

} // namespace vectorbook
