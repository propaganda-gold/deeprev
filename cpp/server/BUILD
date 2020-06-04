load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("//:defs.bzl", "vectorbook_copts", "vectorbook_linkopts")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "simple_server",
    hdrs = [
        "client_http.hpp",
        "client_https.hpp",
        "crypto.hpp",
        "server_http.hpp",
        "server_https.hpp",
        "status_code.hpp",
        "utility.hpp",
    ],
    deps = [
        "//util:includes",
    ],
)

cc_library(
    name = "http_common",
    hdrs = [
        "status_code.hpp",
        "utility.hpp",
    ],
    deps = [],
)

cc_library(
    name = "http_server",
    srcs = [
        "http_server.cc",
    ],
    hdrs = ["http_server.h"],
    deps = [
        ":connection",
        ":http_common",
        "//handlers:api",
        "//handlers:delegating_handler",
        "//handlers:http_cc",
        "//handlers:requests",
        "//util:includes",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "http_read_util",
    srcs = ["http_read_util.cc"],
    hdrs = ["http_read_util.h"],
    deps = ["//util:includes"],
)

cc_library(
    name = "connection",
    srcs = ["connection.cc"],
    hdrs = ["connection.h"],
    deps = [
        "//util:includes",
    ],
)

cc_library(
    name = "options",
    srcs = [
        "options.cc",
    ],
    hdrs = ["options.h"],
    deps = [
        "//server:options_cc",
        "//util:includes",
    ],
)

proto_library(
    name = "options_proto",
    srcs = ["options.proto"],
)

cc_binary(
    name = "rest_server",
    srcs = ["rest_server.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":http_server",
        ":options",
        ":simple_server",
        "//api:router",
        "//data:connection",
        "//data:globals",
        "//data:gold_cc",
        "//network:curl_download",
        "//handlers:api",
        "//handlers:delegating_handler",
        "//handlers:requests",
        "//util:includes",
        "//util:main_incl",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@libboost",
    ],
)

cc_binary(
    name = "debug_server",
    srcs = ["debug_server.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":http_server",
        ":options",
        ":simple_server",
        "//api:router",
        "//data:connection",
        "//data:globals",
        "//data:gold_cc",
        "//handlers:api",
        "//handlers:delegating_handler",
        "//handlers:requests",
        "//network:init",
        "//util:includes",
        "//util:mongo_adapter",
        "//util:main_incl",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@libboost",
    ],
		testonly = 1,
)

cc_binary(
    name = "echo_main",
    srcs = ["echo_main.cc"],
    deps = [
        ":http_server",
        ":options",
        "//handlers:delegating_handler",
        "//util:includes",
        "//util:main_incl",
        "//util:smtp_mail",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "http_server_test",
    srcs = ["http_server_test.cc"],
    deps = [
        ":http_server",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "http_read_util_test",
    srcs = ["http_read_util_test.cc"],
    deps = [
        ":http_read_util",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "connection_test",
    srcs = ["connection_test.cc"],
    deps = [
        ":connection",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "options_test",
    srcs = ["options_test.cc"],
    deps = [
        ":options",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_proto_library(
    name = "options_cc",
    deps = [":options_proto"],
)

cc_binary(
    name = "feed_server",
    srcs = ["feed_server.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":options",
        "//data:connection",
        "//data:globals",
        "//data:gold_cc",
        "//neural:api",
        "//neural:recommendation_publisher",
        "//util:includes",
        "//util:main_incl",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@libboost",
    ],
)

cc_binary(
    name = "train_server",
    srcs = ["train_server.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":options",
        "//data:connection",
        "//data:globals",
        "//data:gold_cc",
        "//neural:action_converter",
        "//neural:api",
        "//neural:boolean_network",
        "//neural:eval",
        "//neural:recommendation_publisher",
        "//util:includes",
        "//util:main_incl",
        "//util:random",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@libboost",
    ],
)

cc_binary(
    name = "event_server",
    srcs = ["event_server.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":options",
        "//data:connection",
        "//data:globals",
        "//data:gold_cc",
        "//neural:action_converter",
        "//neural:api",
        "//neural:boolean_network",
        "//neural:eval",
        "//neural:recommendation_publisher",
        "//util:includes",
        "//util:main_incl",
        "//util:random",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@libboost",
    ],
)

cc_binary(
    name = "simulate_server",
    srcs = ["simulate_server.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":options",
        "//data:connection",
        "//data:globals",
        "//data:gold_cc",
        "//neural:action_converter",
        "//neural:api",
        "//neural:boolean_network",
        "//neural:eval",
        "//neural:recommendation_publisher",
        "//util:includes",
        "//util:main_incl",
        "//util:random",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@libboost",
    ],
)
