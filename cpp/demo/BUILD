load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("//:defs.bzl", "vectorbook_copts", "vectorbook_linkopts")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "torch_so",
    srcs = [
    ],
    alwayslink = 1,
)

cc_library(
    name = "hello_lib",
    srcs = ["hello_lib.cc"],
    hdrs = ["hello_lib.h"],
    deps = ["//util:includes"],
)

cc_binary(
    name = "main",
    srcs = [
        "main.cc",
    ],
    copts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
    ],
    deps = [
        ":torch_so",
    ],
)

cc_binary(
    name = "mnist",
    srcs = ["mnist.cc"],
    copts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
        "-L/usr/lib",
        "-lc10",
        "-ltorch",
        "-pthread",
    ],
    linkopts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
        "-L/usr/lib",
        "-lc10",
        "-ltorch",
        "-lcaffe2_module_test_dynamic",
        "-lcaffe2_detectron_ops",
        "-pthread",
    ],
    deps = [
        ":torch_so",
    ],
)

cc_binary(
    name = "embedding",
    srcs = ["embedding.cc"],
    copts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
        "-L/usr/lib",
        "-lc10",
        "-ltorch",
        "-pthread",
    ],
    linkopts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
        "-L/usr/lib",
        "-lc10",
        "-ltorch",
        "-lcaffe2_module_test_dynamic",
        "-lcaffe2_detectron_ops",
        "-pthread",
    ],
    deps = [
        ":torch_so",
    ],
)

cc_binary(
    name = "torch1",
    srcs = ["torch1.cc"],
    copts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
        "-L/usr/lib",
        "-lc10",
        "-ltorch",
    ],
    linkopts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
        "-L/usr/lib",
        "-lc10",
        "-ltorch",
    ],
    deps = [
    ],
)

cc_binary(
    name = "random1",
    srcs = ["random1.cc"],
    copts = [
    ],
    deps = [
    ],
)

cc_binary(
    name = "breakable",
    srcs = ["breakable.cc"],
    copts = [
    ],
    deps = [
    ],
)

cc_binary(
    name = "fopen1",
    srcs = [
        "fopen1.cc",
    ],
    copts = [
        "-fpermissive",
    ],
    deps = [
        "//util:includes",
        "@libcurl",
    ],
)

cc_binary(
    name = "tensors",
    srcs = ["tensors.cc"],
    copts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
        "-L/usr/lib",
        "-lc10",
        "-ltorch",
        "-pthread",
    ],
    linkopts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
        "-L/usr/lib",
        "-lc10",
        "-ltorch",
        "-lcaffe2_module_test_dynamic",
        "-lcaffe2_detectron_ops",
        "-pthread",
    ],
    deps = [
        "//util:includes",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_binary(
    name = "copy_tensor",
    srcs = ["copy_tensor.cc"],
    copts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
        "-L/usr/lib",
        "-lc10",
        "-ltorch",
        "-pthread",
    ],
    linkopts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
        "-L/usr/lib",
        "-lc10",
        "-ltorch",
        "-lcaffe2_module_test_dynamic",
        "-lcaffe2_detectron_ops",
        "-pthread",
    ],
    deps = [
        "//util:includes",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)

cc_binary(
    name = "logging",
    srcs = ["logging.cc"],
    deps = [
        "//util:includes",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)

cc_binary(
    name = "blocking_ostream",
    srcs = ["blocking_ostream.cc"],
    deps = [
        "//util:includes",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)

cc_binary(
    name = "mutex",
    srcs = ["mutex.cc"],
    deps = [
        "//util:includes",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_binary(
    name = "stacktrace",
    srcs = ["stacktrace.cc"],
    linkopts = ["-ldl"],
    deps = [
        "//util:includes",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@libboost",
    ],
)

cc_binary(
    name = "json_map",
    srcs = ["json_map.cc"],
    deps = [
        "//util:includes",
        "//util:main_incl",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)

cc_binary(
    name = "smtp_mail",
    srcs = ["smtp_mail.cc"],
    deps = [
        "//util:includes",
        "//util:main_incl",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)

cc_binary(
    name = "curl_post",
    srcs = ["curl_post.cc"],
    deps = [
        ":api_cc",
        "//util:includes",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@libcurl",
    ],
)

cc_binary(
    name = "sendgrid_post",
    srcs = ["sendgrid_post.cc"],
    deps = [
        ":api_cc",
        "//util:includes",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@libcurl",
    ],
)

cc_binary(
    name = "bind",
    srcs = ["bind.cc"],
    deps = [
        "//util:includes",
        "//util:main_incl",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)

cc_binary(
    name = "cassandra",
    srcs = ["cassandra.cc"],
    deps = [
        "//util:includes",
        "//util:main_incl",
        "@cassandra",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)

cc_binary(
    name = "mongodb",
    srcs = ["mongodb.cc"],
    deps = [
        "//util:includes",
        "//util:main_incl",
        "@mongodb",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)

cc_binary(
    name = "fopen_short",
    srcs = ["fopen_short.cc"],
    deps = [
        "//util:includes",
        "//util:main_incl",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@libcurl",
    ],
)

cc_binary(
    name = "time",
    srcs = ["time.cc"],
    deps = ["//util:includes",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "//util:main_incl",
		],
)

cc_test(
    name = "hello_lib_test",
    srcs = ["hello_lib_test.cc"],
    deps = [
        ":hello_lib",
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

cc_binary(
    name = "links_from_page",
    srcs = ["links_from_page.cc"],
    deps = [
        "//data:init_ops",
        "//util:download",
        "//util:includes",
        "//util:main_incl",
        "//util:xml",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
		copts = vectorbook_copts,
		linkopts=vectorbook_linkopts,
)
