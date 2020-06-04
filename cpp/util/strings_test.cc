#include "util/strings.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(Replace, Basic ) {
	string s = Replace("hi", {{"hi", "hi"}});
}

} // namespace vectorbook
