#include "util/files.h"

#include "data/vectorbook.pb.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(Json, Basic) {
	User user;
	user.set_id("my_id");
	auto json = ProtoToJsonString(user);
}

} // namespace vectorbook
