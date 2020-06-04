load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "api",
    srcs = ["api.cc"],
    hdrs = [
        "api.h",
        "compliance_incl.h",
    ],
    deps = [
        "//coln:option",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "seq",
    srcs = ["seq.cc"],
    hdrs = ["seq.h"],
    copts = [
        "-std=c++11",
    ],
    linkopts = [
        "-std=c++11",
    ],
    deps = [
        "//coln:api",
    ],
)

cc_library(
    name = "map",
    srcs = ["map.cc"],
    hdrs = ["map.h"],
    deps = [
        ":option",
        "//coln:api",
        "@com_google_absl//absl/types:optional",
    ],
)

cc_library(
    name = "iterable",
    srcs = ["iterable.cc"],
    hdrs = ["iterable.h"],
    deps = [
        ":api",
    ],
)

cc_library(
    name = "set",
    srcs = ["set.cc"],
    hdrs = ["set.h"],
    deps = [
        ":api",
    ],
)

cc_library(
    name = "option",
    srcs = ["option.cc"],
    hdrs = ["option.h"],
    deps = [
        "//util:logging",
        "//util:stacktrace",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
				"@com_google_absl//absl/types:optional",
    ],
)

cc_library(
    name = "util",
    srcs = ["util.cc"],
    hdrs = ["util.h"],
    deps = [
        ":api",
        ":seq",
        "//util:random",
    ],
)

cc_library(
    name = "proto_map",
    srcs = ["proto_map.cc"],
    hdrs = ["proto_map.h"],
    deps = [
        "//util:includes",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_test(
    name = "api_test",
    srcs = ["api_test.cc"],
    deps = [
        ":api",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "seq_test",
    size = "small",
    srcs = ["seq_test.cc"],
    copts = [
        "-std=c++11",
    ],
    linkopts = [
        "-std=c++11",
    ],
    deps = [
        ":seq",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "map_test",
    srcs = ["map_test.cc"],
    deps = [
        ":map",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "iterable_test",
    srcs = ["iterable_test.cc"],
    deps = [
        ":iterable",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "set_test",
    srcs = ["set_test.cc"],
    deps = [
        ":set",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "option_test",
    srcs = ["option_test.cc"],
    deps = [
        ":option",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "util_test",
    srcs = ["util_test.cc"],
    deps = [
        ":util",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "proto_map_test",
    srcs = ["proto_map_test.cc"],
    deps = [
        ":proto_map",
        "//data:gold_cc",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)
