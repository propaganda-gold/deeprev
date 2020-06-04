#include "coln/set.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(Set, Make) {
	auto set = MakeSet<std::string>();
	set->insert("a");
	set->insert("b");
	for (auto i : *set) {
	}
}

TEST(Set, Itbl) {
	auto set = MakeSet<std::string>();
	set->insert("a");
	set->insert("b");
	const ConstIterable<std::string>& itbl = *set;
	for (auto i : itbl) {
	}
}

} // namespace vectorbook
