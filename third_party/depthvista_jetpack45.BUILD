cc_import(
    name = "libdepthvistacamera",
    shared_library = "lib/libdepthvistacamera.so",
)

cc_library(
    name = "depthvista_aarch64_jetpack45",
    hdrs = glob(["include/*.hpp"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":libdepthvistacamera",
        "@libudev_aarch64",
    ],
)
