workspace(name = "com_github_jsonbruce_deductionservice")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Bazel rule for Protocol Buffer
http_archive(
    name = "rules_proto",
    sha256 = "602e7161d9195e50246177e7c55b2f39950a9cf7366f74ed5f22fd45750cd208",
    strip_prefix = "rules_proto-97d8af4dc474595af3900dd85cb3a29ad28cc313",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
        "https://github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
    ],
)
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()


# Protocol Buffer
http_archive(
    name = "com_google_protobuf",
    strip_prefix = "protobuf-master",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/master.zip"],
)
load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
protobuf_deps()


# gRPC 1.29.1
http_archive(
    name = "com_github_grpc_grpc",
    sha256 = "bda7b52bab00592c115d5c2757ca729b665ed39cdf048541bf8aab212464c5a0",
    urls = [
        "https://github.com/grpc/grpc/archive/7d89dbb311f049b43bda7bbf6f7d7bf1b4c24419.tar.gz",
    ],
    strip_prefix = "grpc-7d89dbb311f049b43bda7bbf6f7d7bf1b4c24419",
)
load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()

load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")
grpc_extra_deps()