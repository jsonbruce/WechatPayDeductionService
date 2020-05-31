load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "deduction_proto",
    srcs = ["deduction.proto"],
    deps = [
        "@com_google_protobuf//:timestamp_proto",
    ],
)

cc_proto_library(
    name = "deduction_cc_proto",
    deps = [":deduction_proto"],
)

cc_grpc_library(
    name = "deduction_cc_grpc",
    srcs = [":deduction_proto"],
    deps = [":deduction_cc_proto"],
    grpc_only = True,
)