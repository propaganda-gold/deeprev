load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("//:defs.bzl", "vectorbook_copts", "vectorbook_linkopts")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "broadcast",
    srcs = ["broadcast.cc"],
    hdrs = ["broadcast.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":api_cc",
        ":resource",
        ":time",
        "//data:globals",
        "//data:gold_cc",
        "//util:cassandra_logging",
        "//util:includes",
        "//util:mongo_adapter",
        "//util:protos",
        "//util:re",
        "//util:uuid",
    ],
)

cc_library(
    name = "resource",
    srcs = ["resource.cc"],
    hdrs = ["resource.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":api_cc",
        "//data:globals",
        "//data:gold_cc",
        "//data/ops:image",
        "//util:download",
        "//util:includes",
        "//util:random",
        "//util:uuid",
        "//util:xml",
    ],
)

cc_library(
    name = "user",
    srcs = ["user.cc"],
    hdrs = ["user.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":api_cc",
        "//data:globals",
        "//data:gold_cc",
        "//handlers:api",
        "//handlers:http_cc",
        "//util:includes",
        "//util:uuid",
    ],
)

cc_library(
    name = "invitation",
    srcs = ["invitation.cc"],
    hdrs = ["invitation.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":api_cc",
        ":emailtoken",
        ":time",
        "//data:globals",
        "//handlers:api",
        "//handlers:http_cc",
        "//util:includes",
        "//util:uuid",
    ],
)

cc_library(
    name = "image",
    srcs = ["image.cc"],
    hdrs = ["image.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        "//data:globals",
        "//data:gold_cc",
        "//network:curl_download",
        "//util:cassandra_logging",
        "//util:download",
        "//util:includes",
        "//util:strings",
        "//util:uuid",
    ],
)

cc_library(
    name = "time",
    srcs = ["time.cc"],
    hdrs = ["time.h"],
    deps = [],
)

cc_library(
    name = "emailtoken",
    srcs = ["emailtoken.cc"],
    hdrs = ["emailtoken.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        "//data:globals",
        "//network:send_sendgrid",
        "//util:includes",
        "//util:secure_token",
    ],
)

cc_library(
    name = "cookietoken",
    srcs = ["cookietoken.cc"],
    hdrs = ["cookietoken.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        "//data:globals",
        "//util:includes",
        "//util:secure_token",
    ],
)

cc_library(
    name = "application",
    srcs = ["application.cc"],
    hdrs = ["application.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        "//data:globals",
        "//data:gold_cc",
        "//data/ops:time",
        "//util:includes",
        "//util:uuid",
    ],
)

cc_library(
    name = "applytoken",
    srcs = ["applytoken.cc"],
    hdrs = ["applytoken.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        "//data:globals",
        "//util:includes",
        "//util:secure_token",
    ],
)

cc_library(
    name = "channel",
    srcs = ["channel.cc"],
    hdrs = ["channel.h"],
    deps = ["//util:includes"],
)

cc_test(
    name = "broadcast_test",
    srcs = ["broadcast_test.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":broadcast",
        "//data:connection",
        "//data:test_only",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "resource_test",
    srcs = ["resource_test.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":resource",
        "//data:connection",
        "//data:test_only",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "user_test",
    srcs = ["user_test.cc"],
    deps = [
        ":user",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "invitation_test",
    srcs = ["invitation_test.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":invitation",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "image_test",
    srcs = ["image_test.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":image",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "time_test",
    srcs = ["time_test.cc"],
    deps = [
        ":time",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "emailtoken_test",
    srcs = ["emailtoken_test.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":emailtoken",
        "//server:options",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "cookietoken_test",
    srcs = ["cookietoken_test.cc"],
    deps = [
        ":cookietoken",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "application_test",
    srcs = ["application_test.cc"],
    deps = [
        ":application",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "applytoken_test",
    srcs = ["applytoken_test.cc"],
    deps = [
        ":applytoken",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "channel_test",
    srcs = ["channel_test.cc"],
    deps = [
        ":channel",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

proto_library(
    name = "api",
    srcs = ["api.proto"],
)

cc_proto_library(
    name = "api_cc",
    deps = [":api"],
)
