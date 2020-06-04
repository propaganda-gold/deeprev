#include "util/check.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

Unique<int> MyConst() {
	RAISE_EXCEPTION("My error");
}

Void MyVoid() {
	auto ci = MyConst();
	Check(ci);
	int s = *ci;
	return Ok();
}

TEST(Check, Basic) {
	auto v = MyVoid();
}

} // namespace vectorbook
