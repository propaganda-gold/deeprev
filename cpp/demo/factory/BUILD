load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("//:defs.bzl", "vectorbook_copts", "vectorbook_linkopts")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "factory_demo",
    srcs = [
		"Base.h",
		"BaseFactory.h",
		"BaseImpl.h",
		"Base.cpp",
		"BaseFactory.cpp",
		"factory_demo.cc",
		],
    deps = ["//util:includes"],
)
