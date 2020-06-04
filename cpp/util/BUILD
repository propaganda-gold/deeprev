load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")
load("//:defs.bzl", "vectorbook_copts", "vectorbook_linkopts")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "includes",
    srcs = ["includes.cc"],
    hdrs = ["includes.h"],
    deps = [
        ":files",
        ":logging",
        ":strings",
        "//coln:api",
        "//coln:map",
        "//coln:seq",
        "//coln:set",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_library(
    name = "files",
    srcs = ["files.cc"],
    hdrs = ["files.h"],
    deps = [
        ":logging",
        ":strings",
        "//coln:option",
        "@com_google_absl//absl/strings",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_library(
    name = "strings",
    srcs = ["strings.cc"],
    hdrs = ["strings.h"],
    copts = vectorbook_copts,
    deps = [
        ":logging",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "col",
    srcs = ["col.cc"],
    hdrs = ["col.h"],
    deps = [
        ":includes",
        "@com_google_absl//absl/types:optional",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_test(
    name = "files_test",
    srcs = ["files_test.cc"],
    deps = [
        ":files",
        ":includes",
        "//data:gold_cc",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "strings_test",
    size = "small",
    srcs = ["strings_test.cc"],
    copts = vectorbook_copts,
    deps = [
        ":includes",
        ":strings",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "col_test",
    srcs = ["col_test.cc"],
    deps = [
        ":col",
        ":includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "logging_test",
    srcs = ["logging_test.cc"],
    deps = [
        ":includes",
        ":logging",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "download_test",
    srcs = ["download_test.cc"],
    deps = [
        ":download",
        ":includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "xml_test",
    size = "small",
    srcs = ["xml_test.cc"],
    deps = [
        ":download",
        ":includes",
        ":xml",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "re_test",
    srcs = ["re_test.cc"],
    deps = [
        ":includes",
        ":re",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "iterable_test",
    srcs = ["iterable_test.cc"],
    deps = [
        ":includes",
        ":iterable",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "random_test",
    srcs = ["random_test.cc"],
    deps = [
        ":includes",
        ":random",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "stacktrace_test",
    srcs = ["stacktrace_test.cc"],
    linkopts = ["-ldl"],
    deps = [
        ":includes",
        ":stacktrace",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "check_test",
    srcs = ["check_test.cc"],
    deps = [
        ":check",
        ":includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "html",
    srcs = ["html.cc"],
    hdrs = ["html.h"],
    deps = [
        ":col",
        ":includes",
        ":strings",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_library(
    name = "logging",
    srcs = ["logging.cc"],
    hdrs = ["logging.h"],
    deps = [
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
    ],
)

cc_library(
    name = "download",
    srcs = ["download.cc"],
    hdrs = ["download.h"],
    copts = ["-fpermissive"],
    deps = [
        ":includes",
        "@libcurl",
    ],
)

cc_library(
    name = "rapidxml",
    hdrs = [
        "rapidxml.hpp",
        "rapidxml_iterators.hpp",
        "rapidxml_print.hpp",
        "rapidxml_utils.hpp",
    ],
)

cc_library(
    name = "xml",
    srcs = ["xml.cc"],
    hdrs = ["xml.h"],
    deps = [
        ":includes",
        ":rapidxml",
        "//coln:util",
    ],
)

cc_library(
    name = "re",
    srcs = ["re.cc"],
    hdrs = ["re.h"],
    deps = [":includes"],
)

cc_library(
    name = "iterable",
    srcs = ["iterable.cc"],
    hdrs = ["iterable.h"],
    deps = [":includes"],
)

cc_library(
    name = "random",
    srcs = ["random.cc"],
    hdrs = ["random.h"],
    deps = ["//coln:api"],
)

cc_library(
    name = "stacktrace",
    srcs = ["stacktrace.cc"],
    hdrs = ["stacktrace.h"],
    linkopts = ["-ldl"],
    deps = [
        "@libboost",
    ],
)

cc_library(
    name = "check",
    srcs = ["check.cc"],
    hdrs = ["check.h"],
    deps = [
        ":includes",
        "//coln:api",
    ],
)

cc_test(
    name = "html_test",
    srcs = ["html_test.cc"],
    deps = [
        ":html",
        ":includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "protos_test",
    srcs = ["protos_test.cc"],
    deps = [
        ":includes",
        ":protos",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "smtp_mail_test",
    srcs = ["smtp_mail_test.cc"],
    deps = [
        ":includes",
        ":protos",
        ":smtp_mail",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "registry_test",
    srcs = ["registry_test.cc"],
    deps = [
        ":includes",
        ":registry",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "rate_limiter_test",
    srcs = ["rate_limiter_test.cc"],
    deps = [
        ":includes",
        ":rate_limiter",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "curl_post_test",
    srcs = ["curl_post_test.cc"],
    deps = [
        ":curl_post",
        ":includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "check_recaptcha_test",
    srcs = ["check_recaptcha_test.cc"],
    deps = [
        ":check_recaptcha",
        ":includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "send_sendgrid_test",
    srcs = ["send_sendgrid_test.cc"],
    deps = [
        ":includes",
        ":send_sendgrid",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "cassandra_logging_test",
    srcs = ["cassandra_logging_test.cc"],
    deps = [
        ":cassandra_logging",
        ":includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "uuid_test",
    srcs = ["uuid_test.cc"],
    deps = [
        ":includes",
        ":uuid",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "secure_token_test",
    srcs = ["secure_token_test.cc"],
    deps = [
        ":includes",
        ":secure_token",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "mongo_adapter_test",
    srcs = ["mongo_adapter_test.cc"],
    deps = [
        ":includes",
        ":mongo_adapter",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "main_incl",
    hdrs = ["main_incl.h"],
)

cc_library(
    name = "protos",
    srcs = ["protos.cc"],
    hdrs = ["protos.h"],
    deps = [":includes"],
)

cc_library(
    name = "smtp_mail",
    srcs = ["smtp_mail.cc"],
    hdrs = ["smtp_mail.h"],
    deps = [
        ":includes",
        ":smtp_cc",
        ":strings",
        "@libcurl",
    ],
)

cc_library(
    name = "registry",
    srcs = ["registry.cc"],
    hdrs = ["registry.h"],
    deps = [":includes"],
)

cc_library(
    name = "rate_limiter",
    srcs = ["rate_limiter.cc"],
    hdrs = ["rate_limiter.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":includes",
        ":rate_limiter_options_cc",
        "//coln:api",
        "//coln:map",
        "//data:map",
    ],
)

cc_library(
    name = "curl_post",
    srcs = ["curl_post.cc"],
    hdrs = ["curl_post.h"],
    deps = [":includes"],
)

cc_library(
    name = "check_recaptcha",
    srcs = ["check_recaptcha.cc"],
    hdrs = ["check_recaptcha.h"],
    deps = [
        ":includes",
        ":smtp_cc",
        "//server:options",
        "@libcurl",
    ],
)

cc_library(
    name = "send_sendgrid",
    srcs = ["send_sendgrid.cc"],
    hdrs = ["send_sendgrid.h"],
    deps = [":includes"],
)

cc_library(
    name = "cassandra_logging",
    srcs = ["cassandra_logging.cc"],
    hdrs = ["cassandra_logging.h"],
    deps = [
        ":includes",
        ":protos",
        ":strings",
        "@cassandra",
    ],
)

cc_library(
    name = "uuid",
    srcs = ["uuid.cc"],
    hdrs = ["uuid.h"],
    deps = [
        ":includes",
        ":random",
        ":strings",
        "@cassandra",
    ],
)

cc_library(
    name = "secure_token",
    srcs = ["secure_token.cc"],
    hdrs = ["secure_token.h"],
    deps = [
        ":includes",
        ":random",
    ],
)

cc_library(
    name = "mongo_adapter",
    srcs = ["mongo_adapter.cc"],
    hdrs = ["mongo_adapter.h"],
    deps = [
				":includes",
				"//server:options",
        "@mongodb",
		],
)

proto_library(
    name = "smtp",
    srcs = ["smtp.proto"],
)

proto_library(
    name = "rate_limiter_options",
    srcs = ["rate_limiter_options.proto"],
)

cc_proto_library(
    name = "smtp_cc",
    deps = [":smtp"],
)

cc_proto_library(
    name = "rate_limiter_options_cc",
    deps = [":rate_limiter_options"],
)

cc_binary(
    name = "rate_limiter_harness",
    srcs = ["rate_limiter_harness.cc"],
    deps = [
        ":includes",
        ":main_incl",
        ":rate_limiter",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)
