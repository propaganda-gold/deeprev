#include "coln/option.h"

#include <iostream>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

//class Void : public Checkable {
//public:
//  Void() {}
//  Void(const Null &null) : Checkable () {}
//  Void(const Error &error) : Checkable(error) {}
//	std::string msg() { return "hi";}
//};

TEST(Void, Basic) {
	// Void v; //  (Null());
	Void v (Null{});
	std::cout << v.status() << std::endl;
	// std::cout << v.status() << std::endl;
}

TEST(Void, Basic2) {
	// Void v; //  (Null());
	Void v (Error("file", 2, "Test error"));
	std::cout << v.status() << std::endl;
	// std::cout << v.status() << std::endl;
}

TEST(Unique, Basic) {
	Unique<int> r (new int());
	std::cout << r.status() << '\n';
}
TEST(Unique, Basic1) {
	Unique<int> r (Null{});
	std::cout << r.status() << '\n';
}

Unique<int> MyFn() {
	Unique<int> r (nullptr);
	ASSERT_EXISTS(r);
	return r;
}

TEST(Unique, Basic2) {
	Unique<int> r = MyFn();
	std::cout << r.status() << '\n';
	std::cout << r.has_error() << '\n';

}

} // namespace vectorbook
