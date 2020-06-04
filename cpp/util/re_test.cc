#include "util/re.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(FindAllMatches, Basic) {

	vector<string> matches = re::FindAllMatches("hi there hi", "hi");
}

} // namespace vectorbook
