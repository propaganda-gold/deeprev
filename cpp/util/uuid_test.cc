#include "util/uuid.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(Uuid, Basic) {
	auto uuid = FreshTimeUUID();
	ASSERT_TRUE(uuid.ok());
}
TEST(Uuid, Basic2) {
	auto uuid = FreshTimeUUID();
	ASSERT_TRUE(uuid.ok());
}

} // namespace vectorbook
