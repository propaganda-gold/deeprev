#include "data/ops/link.h"

#include "data/connection.h"
#include "data/test_only.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(ResourceIdFromUrl, Url) {
	auto create = CreateRedisConnection();
	ASSERT_TRUE(create.ok());

	auto clear = ClearLocalDatabase();
	ASSERT_TRUE(clear.ok());

	auto resource = ResourceFromUrl("https://www.youtube.com/watch?v=s8V6NxdmpO0");
	ASSERT_TRUE(resource.ok());

}

} // namespace vectorbook
