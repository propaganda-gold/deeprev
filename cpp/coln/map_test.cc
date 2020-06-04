#include "coln/map.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

//TEST(Map, Make) {
//	auto map = MakeMap<std::string, int>();
//	map->put("hi", 0);
//	map->put("there", 1);
//	for (auto& kv : *map) {
//	}
//}

std::function<int()> EmptyInt() {
	return []() { return 0; };
}
TEST(Map, GetOrUpdate) {
	auto map = MakeMap<std::string, int>();
//	const int* i = map->const_get_or_update("hi", EmptyInt()); 
}

TEST(Map, Iterate) {
	auto map = MakeMap<std::string, int>(EmptyInt());
	*map->mutable_get("john") += 1;

	for (auto kv : *map) {
	}
}

TEST(ConstMap, Iterate) {
	ConstMap<std::string, std::string> map;
	std::string hi = "hi";
	map.put("a", std::move(hi));
	auto size = map.size();
	ASSERT_TRUE(size.ok());
}

} // namespace vectorbook
