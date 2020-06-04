load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("//:defs.bzl", "vectorbook_copts", "vectorbook_linkopts")

package(
    default_visibility = ["//visibility:public"],
)

cc_proto_library(
    name = "features_cc",
    deps = ["features"],
)

proto_library(
    name = "features",
    srcs = ["features.proto"],
		deps = [
			"//data:vectorbook",
		]
)

cc_proto_library(
    name = "training_config_cc",
    deps = ["training_config"],
)

proto_library(
    name = "training_config",
    srcs = ["training_config.proto"],
)

cc_binary(
    name = "train1",
    srcs = ["train1.cc"],
    copts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
        "-I/usr/local/include/hiredis",
    ],
    deps = [
        ":basic_model",
        ":eval",
        ":inference",
        ":training_config_cc",
        "//neural:feature_fn",
        "//neural:features_cc",
        "//site:annotations_cc",
        "//util:includes",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
    ],
)

cc_binary(
    name = "train2",
    srcs = ["train2.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":basic_model",
        ":eval",
        ":inference",
        ":training_config_cc",
        "//neural:feature_fn",
        "//neural:features_cc",
        "//site:annotations_cc",
        "//util:includes",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
    ],
)

cc_binary(
    name = "train3",
    srcs = ["train3.cc"],
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
        ":api",
        ":basic_model",
        ":eval",
        ":store_embeddings",
        ":training_config_cc",
        "//neural:feature_fn",
        "//neural:features_cc",
        "//site:annotations_cc",
        "//util:includes",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "feature_fn",
    srcs = ["feature_fn.cc"],
    hdrs = ["feature_fn.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":features_cc",
        ":training_config_cc",
        "//coln:api",
        "//data:globals",
        "//data:gold_cc",
        "//site:annotations_cc",
        "//util:includes",
    ],
)

cc_library(
    name = "inference",
    srcs = ["inference.cc"],
    hdrs = ["inference.h"],
    copts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
        "-I/usr/local/include/hiredis",
    ],
    deps = [
        ":api",
        ":features_cc",
        "//coln:api",
        "//coln:map",
        "//data:globals",
        "//demo:torch_so",
        "//util:includes",
    ],
)

cc_library(
    name = "labels",
    srcs = ["labels.cc"],
    hdrs = ["labels.h"],
    deps = [
        "//site:annotations_cc",
        "//util:includes",
    ],
)

cc_library(
    name = "index_manager",
    srcs = ["index_manager.cc"],
    hdrs = ["index_manager.h"],
    copts = [
        "-I/usr/local/include/hiredis",
        "-fpermissive",
    ],
    deps = [
        "//data:globals",
        "//util:includes",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "eval",
    srcs = ["eval.cc"],
    hdrs = ["eval.h"],
    deps = [
        ":features_cc",
        "//util:includes",
    ],
)

cc_library(
    name = "basic_model",
    srcs = ["basic_model.cc"],
    hdrs = ["basic_model.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":api",
        ":embedding_layer",
        ":store_embeddings",
        ":training_config_cc",
        "//coln:api",
        "//coln:map",
        "//util:includes",
        "@libtorch",
    ],
)

cc_library(
    name = "store_embeddings",
    srcs = ["store_embeddings.cc"],
    hdrs = ["store_embeddings.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":features_cc",
        ":training_config_cc",
        "//coln:api",
        "//data:globals",
        "//data:gold_cc",
        "//data:keys",
        "//demo:torch_so",
        "//site:annotations_cc",
        "//util:includes",
    ],
)

cc_library(
    name = "api",
    srcs = ["api.cc"],
    hdrs = ["api.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":features_cc",
        "//coln:api",
        "//data:gold_cc",
        "//util:includes",
    ],
)

cc_library(
    name = "generator",
    srcs = ["generator.cc"],
    hdrs = ["generator.h"],
    deps = ["//util:includes"],
)

cc_library(
    name = "embedding_layer",
    srcs = ["embedding_layer.cc"],
    hdrs = ["embedding_layer.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":api",
        ":store_embeddings",
        ":training_config_cc",
        "//coln:api",
        "//coln:map",
        "//util:includes",
    ],
)

cc_library(
    name = "clustering",
    srcs = ["clustering.cc"],
    hdrs = ["clustering.h"],
    deps = [
        "//coln:map",
        "//coln:seq",
        "//coln:set",
        "//site:annotations_cc",
        "//util:includes",
    ],
)

cc_library(
    name = "example_generator",
    srcs = ["example_generator.cc"],
    hdrs = ["example_generator.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":api",
        ":feature_fn",
        ":training_config_cc",
        "//coln:api",
        "//coln:util",
        "//data:globals",
        "//util:includes",
        "//util:random",
    ],
)

cc_library(
    name = "output_layer",
    srcs = ["output_layer.cc"],
    hdrs = ["output_layer.h"],
    deps = ["//util:includes"],
)

cc_library(
    name = "recommendation_publisher",
    srcs = ["recommendation_publisher.cc"],
    hdrs = ["recommendation_publisher.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        "//data:globals",
        "//server:options",
        "//util:includes",
    ],
)

cc_library(
    name = "recommendation_maker",
    srcs = ["recommendation_maker.cc"],
    hdrs = ["recommendation_maker.h"],
    deps = ["//util:includes"],
)

cc_library(
    name = "globals",
    srcs = ["globals.cc"],
    hdrs = ["globals.h"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        "//util:includes",
    ],
)

cc_library(
    name = "boolean_network",
    srcs = ["boolean_network.cc"],
    hdrs = ["boolean_network.h"],
    deps = ["//util:includes", "//neural:api", "//neural:basic_model"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_library(
    name = "action_converter",
    srcs = ["action_converter.cc"],
    hdrs = ["action_converter.h"],
    deps = [
        ":api",
        "//util:includes",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_test(
    name = "store_embeddings_test",
    srcs = ["store_embeddings_test.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":store_embeddings",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "feature_fn_test",
    srcs = ["feature_fn_test.cc"],
    deps = [
        ":feature_fn",
        "//site:annotations_cc",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "inference_test",
    srcs = ["inference_test.cc"],
    copts = [
        "-fpermissive",
        "-I/usr/include/torch/csrc/api/include",
        "-I/usr/include",
    ],
    deps = [
        ":inference",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "labels_test",
    srcs = ["labels_test.cc"],
    deps = [
        ":labels",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "index_manager_test",
    srcs = ["index_manager_test.cc"],
    deps = [
        ":index_manager",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "eval_test",
    srcs = ["eval_test.cc"],
    deps = [
        ":eval",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "basic_model_test",
    srcs = ["basic_model_test.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":basic_model",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "api_test",
    srcs = ["api_test.cc"],
    deps = [
        ":api",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "generator_test",
    srcs = ["generator_test.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":generator",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "embedding_layer_test",
    srcs = ["embedding_layer_test.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":embedding_layer",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "clustering_test",
    srcs = ["clustering_test.cc"],
    deps = [
        ":clustering",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "example_generator_test",
    srcs = ["example_generator_test.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":example_generator",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "output_layer_test",
    srcs = ["output_layer_test.cc"],
    deps = [
        ":output_layer",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "recommendation_publisher_test",
    srcs = ["recommendation_publisher_test.cc"],
    deps = [
        ":recommendation_publisher",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "recommendation_maker_test",
    srcs = ["recommendation_maker_test.cc"],
    deps = [
        ":recommendation_maker",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "globals_test",
    srcs = ["globals_test.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":globals",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "boolean_network_test",
    srcs = ["boolean_network_test.cc"],
    deps = [
        ":boolean_network",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_test(
    name = "action_converter_test",
    srcs = ["action_converter_test.cc"],
    deps = [
        ":action_converter",
        "//util:includes",
        "@com_google_googletest//:gtest_main",
    ],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
)

cc_binary(
    name = "train",
    srcs = ["train.cc"],
    copts = vectorbook_copts,
    linkopts = vectorbook_linkopts,
    deps = [
        ":api",
        ":basic_model",
        ":eval",
        ":example_generator",
        ":store_embeddings",
        ":training_config_cc",
        "//data:globals",
        "//neural:feature_fn",
        "//neural:features_cc",
        "//site:annotations_cc",
        "//util:includes",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
        "@libboost",
    ],
)
