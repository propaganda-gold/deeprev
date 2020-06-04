load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("//:defs.bzl", "vectorbook_copts", "vectorbook_linkopts")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "http_cc",
    deps = ["http"],
)

proto_library(
    name = "http",
    srcs = ["http.proto"],
)

cc_library(
    name = "api",
    srcs = ["api.cc"],
    hdrs = ["api.h"],
    deps = [
        ":http_cc",
        "//api:responses_cc",
        "//data:gold_cc",
        "//data:globals",
        "//util:col",
        "//util:includes",
        "//util:protos",
        "//util:strings",
        "@com_google_protobuf//:protobuf_lite",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_library(
    name = "predicate_handler",
    srcs = ["predicate_handler.cc"],
    hdrs = ["predicate_handler.h"],
    deps = [
        ":api",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_library(
    name = "delegating_handler",
    srcs = ["delegating_handler.cc"],
    hdrs = ["delegating_handler.h"],
    copts = ["-fpermissive"],
    deps = [
        ":api",
        "//util:includes",
        "@com_google_absl//absl/strings",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_library(
    name = "authentication_predicates",
    srcs = ["authentication_predicates.cc"],
    hdrs = ["authentication_predicates.h"],
    deps = [
        ":api",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_protobuf//:protobuf_lite",
        "@hiredis",
        "@libboost",
        "@libcurl",
    ],
)

cc_library(
    name = "requests",
    srcs = ["requests.cc"],
    hdrs = ["requests.h"],
    deps = [
        ":api",
        ":http_cc",
        "//util:includes",
        "@com_google_absl//absl/strings",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_test(
    name = "requests_test",
    size = "small",
    srcs = ["requests_test.cc"],
    deps = [
        ":requests",
        "@com_google_googletest//:gtest_main",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_binary(
    name = "server_main",
    srcs = ["server_main.cc"],
    deps = [
        ":api",
        ":requests",
        "//util:includes",
    ],
)

cc_library(
    name = "http_server",
    srcs = ["http_server.cc"],
    hdrs = ["http_server.h"],
    deps = [
        ":api",
        ":requests",
        "//pages:gold_root",
        "//util:includes",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_protobuf//:protobuf_lite",
        "@hiredis",
        "@libboost",
        "@libcpprest",
        "@libcurl",
    ],
)
