load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("//:defs.bzl", "vectorbook_copts", "vectorbook_linkopts")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "redis_cc",
    deps = ["redis"],
)

proto_library(
    name = "redis",
    srcs = ["redis.proto"],
)

# DEPRECATED
cc_proto_library(
    name = "gold_cc",
    deps = ["vectorbook"],
)

proto_library(
    name = "vectorbook",
    srcs = ["vectorbook.proto"],
)

cc_library(
    name = "connection",
    srcs = ["connection.cc"],
    hdrs = ["connection.h"],
    deps = [
        ":redis_cc",
        "//server:options",
        "//util:includes",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_protobuf//:protobuf_lite",
        "@hiredis",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_test(
    name = "connection_test",
    size = "small",
    srcs = ["connection_test.cc"],
    deps = [
        ":connection",
        ":redis_cc",
        "@com_google_googletest//:gtest_main",
        "@hiredis",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_test(
    name = "globals_test",
    srcs = ["globals_test.cc"],
    deps = [
        ":globals",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_test(
    name = "map_test",
    srcs = ["map_test.cc"],
    deps = [
        ":gold_cc",
        ":map",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_test(
    name = "seq_test",
    srcs = ["seq_test.cc"],
    deps = [
        ":seq",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_test(
    name = "set_test",
    srcs = ["set_test.cc"],
    deps = [
        ":set",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_test(
    name = "converter_test",
    srcs = ["converter_test.cc"],
    deps = [
        ":converter",
        "//data:gold_cc",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_test(
    name = "api_test",
    srcs = ["api_test.cc"],
    deps = [
        ":api",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_test(
    name = "test_only_test",
    srcs = ["test_only_test.cc"],
    deps = [
        ":test_only",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_library(
    name = "globals",
    srcs = ["globals.cc"],
    hdrs = ["globals.h"],
    deps = [
        ":api",
        ":gold_cc",
        ":map",
        ":seq",
        ":set",
        "//coln:map",
        "//neural:training_config_cc",
        "//util:includes",
        "//util:smtp_cc",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_library(
    name = "map",
    srcs = ["map.cc"],
    hdrs = ["map.h"],
    deps = [
        ":connection",
        ":converter",
        "//coln:api",
        "//coln:map",
        "//util:includes",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_library(
    name = "seq",
    srcs = ["seq.cc"],
    hdrs = ["seq.h"],
    deps = [
        ":connection",
        ":converter",
        "//coln:api",
        "//coln:seq",
        "//util:includes",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_library(
    name = "set",
    srcs = ["set.cc"],
    hdrs = ["set.h"],
    deps = [
        ":connection",
        "//coln:api",
        "//coln:set",
        "//util:includes",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_library(
    name = "converter",
    srcs = ["converter.cc"],
    hdrs = ["converter.h"],
    deps = ["//util:includes"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_library(
    name = "api",
    srcs = ["api.cc"],
    hdrs = ["api.h"],
    deps = ["//util:includes"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_library(
    name = "test_only",
    srcs = ["test_only.cc"],
    hdrs = ["test_only.h"],
    deps = [
			"//util:includes",
			"//data:connection",
		],
		testonly = 1,
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)
