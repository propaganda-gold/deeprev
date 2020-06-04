load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("//:defs.bzl", "vectorbook_copts", "vectorbook_linkopts")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "signup_start",
    srcs = ["signup_start.cc"],
    hdrs = ["signup_start.h"],
    deps = ["//util:includes"],
)

cc_library(
    name = "signup_validate",
    srcs = ["signup_validate.cc"],
    hdrs = ["signup_validate.h"],
    deps = ["//util:includes"],
)

cc_library(
    name = "signup_finish",
    srcs = ["signup_finish.cc"],
    hdrs = ["signup_finish.h"],
    deps = ["//util:includes"],
)

cc_library(
    name = "router",
    # TODO(greg) remove this, factor into prod/debug flows
    testonly = 1,
    srcs = ["router.cc"],
    hdrs = ["router.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":application",
        ":responses_cc",
        "//api:broadcast",
        "//api:cookietoken",
        "//api:emailtoken",
        "//api:feed",
        "//api:invitation",
        "//api:label",
        "//api:resource",
        "//api:user",
        "//handlers:api",
        "//handlers:delegating_handler",
        "//handlers:requests",
        "//util:includes",
    ],
)

cc_library(
    name = "emailtoken",
    srcs = ["emailtoken.cc"],
    hdrs = ["emailtoken.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":responses_cc",
        "//data:connection",
        "//data:globals",
        "//data/ops:emailtoken",
        "//data/ops:user",
        "//handlers:api",
        "//handlers:http_cc",
        "//server:options",
        "//util:includes",
        "//util:secure_token",
        "//util:uuid",
    ],
)

cc_library(
    name = "user",
    srcs = ["user.cc"],
    hdrs = ["user.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":responses_cc",
        "//data:connection",
        "//data:globals",
        "//data/ops:cookietoken",
        "//data/ops:emailtoken",
        "//data/ops:user",
        "//handlers:api",
        "//handlers:http_cc",
        "//util:includes",
        "//util:random",
    ],
)

cc_library(
    name = "cookietoken",
    srcs = ["cookietoken.cc"],
    hdrs = ["cookietoken.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":responses_cc",
        "//data:connection",
        "//data:globals",
        "//data/ops:user",
        "//handlers:api",
        "//handlers:http_cc",
        "//util:includes",
        "//util:secure_token",
        "//util:uuid",
    ],
)

cc_library(
    name = "broadcast",
    srcs = ["broadcast.cc"],
    hdrs = ["broadcast.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        "//data:globals",
        "//data:converter",
        "//data/ops:broadcast",
        "//handlers:api",
        "//handlers:http_cc",
        "//util:cassandra_logging",
        "//util:includes",
        "//util:random",
    ],
)

cc_library(
    name = "label",
    srcs = ["label.cc"],
    hdrs = ["label.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        "//data:globals",
        "//data/ops:broadcast",
        "//handlers:api",
        "//handlers:http_cc",
        "//util:includes",
        "//util:uuid",
    ],
)

cc_library(
    name = "feed",
    srcs = ["feed.cc"],
    hdrs = ["feed.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        "//data:globals",
        "//data/ops:broadcast",
        "//handlers:api",
        "//handlers:http_cc",
        "//util:includes",
    ],
)

cc_library(
    name = "invitation",
    srcs = ["invitation.cc"],
    hdrs = ["invitation.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        "//data/ops:api_cc",
        "//data/ops:emailtoken",
        "//data/ops:invitation",
        "//handlers:api",
        "//util:includes",
        "//util:random",
    ],
)

cc_library(
    name = "application",
    srcs = ["application.cc"],
    hdrs = ["application.h"],
    deps = [
        ":responses_cc",
        "//data/ops:application",
        "//data/ops:applytoken",
        "//handlers:api",
        "//server:options",
        "//util:check_recaptcha",
        "//util:includes",
        "//util:smtp_cc",
    ],
)

cc_library(
    name = "resource",
    srcs = ["resource.cc"],
    hdrs = ["resource.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":responses_cc",
        "//data:globals",
        "//data/ops:resource",
        "//handlers:api",
        "//handlers:http_cc",
        "//util:includes",
        "//util:random",
    ],
)

cc_library(
    name = "channel",
    srcs = ["channel.cc"],
    hdrs = ["channel.h"],
    deps = ["//util:includes"],
)

cc_test(
    name = "signup_start_test",
    srcs = ["signup_start_test.cc"],
    deps = [
        ":signup_start",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "signup_validate_test",
    srcs = ["signup_validate_test.cc"],
    deps = [
        ":signup_validate",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "signup_finish_test",
    srcs = ["signup_finish_test.cc"],
    deps = [
        ":signup_finish",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "router_test",
    srcs = ["router_test.cc"],
    deps = [
        ":router",
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
    name = "cookietoken_test",
    srcs = ["cookietoken_test.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":cookietoken",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "broadcast_test",
    srcs = ["broadcast_test.cc"],
    deps = [
        ":broadcast",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "label_test",
    srcs = ["label_test.cc"],
    deps = [
        ":label",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "feed_test",
    srcs = ["feed_test.cc"],
    deps = [
        ":feed",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "debug_test",
    srcs = ["debug_test.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":debug",
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
    name = "application_test",
    srcs = ["application_test.cc"],
    deps = [
        ":application",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "resource_test",
    srcs = ["resource_test.cc"],
    deps = [
        ":resource",
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
    name = "responses",
    srcs = ["responses.proto"],
    deps = ["//data:vectorbook"],
)

cc_proto_library(
    name = "responses_cc",
    deps = [":responses"],
)
