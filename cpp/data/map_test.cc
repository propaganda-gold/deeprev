#include "data/map.h"

#include "data/vectorbook.pb.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {
namespace data {

TEST(Seq, Make) {
  auto map = RedisStringMap("hello");
	map->put("hi", "there");
	for (auto pair : *map) {
	}
}

} // namespace data
} // namespace vectorbook
