load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

vectorbook_copts = [
	"-fpermissive",
	"-I/usr/include/torch/csrc/api/include",
	"-I/usr/include",
	"-I/usr/local/include/hiredis",
	"-L/usr/lib",
	"-lc10",
	"-ltorch",
	"-pthread",
]

vectorbook_linkopts = [
	"-fpermissive",
	"-I/usr/include/torch/csrc/api/include",
	"-I/usr/include",
	"-L/usr/lib",
	"-L/home/greg/libtorch/lib",
	"-L/home/greg/hiredis",
	"-lc10",
	"-ltorch",
	"-lcaffe2_module_test_dynamic",
	"-lcaffe2_detectron_ops",
	"-pthread",
]

