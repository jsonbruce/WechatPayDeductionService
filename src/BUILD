load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "deduction_server",
    srcs = [
        "deduction_server.cc",
        "helper.cc",
        "helper.h",
    ],
    defines = ["BAZEL_BUILD"],
    deps = [
        "//api:deduction_cc_grpc", 
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
    ],
    copts = ["-std=c++17"],
)

cc_binary(
    name = "deduction_client",
    srcs = [
        "deduction_client.cc",
        "helper.cc",
        "helper.h",
    ],
    defines = ["BAZEL_BUILD"],
    deps = [
        "//api:deduction_cc_grpc", 
        "@com_github_grpc_grpc//:grpc++",
    ],
    copts = ["-std=c++17"],
)

