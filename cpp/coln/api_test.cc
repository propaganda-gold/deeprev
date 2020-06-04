#include "coln/api.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(Error, Basic) {
	Error error("a", 1, "hello");
	auto& e = error.add_location("hi", 2);
}

Void MyFn() {
	Error error("b", 2, "hello");
	return error;
}

TEST(Check, Basic) {
	Error error("c", 4, "hello");
	Void v = error;
}

TEST(Check, Basic2) {
	Void v = MyFn();
}

TEST(Boolean, Api) {
	Boolean b;
}

} // namespace vectorbook
