load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("//:defs.bzl", "vectorbook_copts", "vectorbook_linkopts")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "send_sendgrid",
    srcs = ["send_sendgrid.cc"],
    hdrs = ["send_sendgrid.h"],
    deps = [
        ":requests_cc",
        "//util:includes",
        "@libcurl",
    ],
)

cc_library(
    name = "init",
    srcs = ["init.cc"],
    hdrs = ["init.h"],
    deps = [
				"//util:includes",
				"@libcurl",
		],
)

cc_library(
    name = "curl_download",
    srcs = ["curl_download.cc"],
    hdrs = ["curl_download.h"],
    deps = [
        "//util:includes",
        "@libcurl",
    ],
)

cc_test(
    name = "send_sendgrid_test",
    srcs = ["send_sendgrid_test.cc"],
    deps = [
        ":send_sendgrid",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "init_test",
    srcs = ["init_test.cc"],
    deps = [
        ":init",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "curl_download_test",
    srcs = ["curl_download_test.cc"],
    deps = [
        ":curl_download",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

proto_library(
    name = "requests",
    srcs = ["requests.proto"],
)

cc_proto_library(
    name = "requests_cc",
    deps = [":requests"],
)

cc_binary(
    name = "sendgrid_main",
    srcs = ["sendgrid_main.cc"],
    deps = [
        ":send_sendgrid",
        "//util:includes",
        "//util:main_incl",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)
