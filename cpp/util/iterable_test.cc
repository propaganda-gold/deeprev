#include "util/iterable.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(Iteratable, Basic) {
	// vector<int> stl {1};
	vector<int> stl {};
	VectorIterator<int> vi (&stl);
	Iterable<int> iterable(&vi);

	for (int i : iterable) {
	}
}

} // namespace vectorbook
