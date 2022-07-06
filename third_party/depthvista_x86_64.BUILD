cc_import(
    name = "libdepthvistacamera",
    shared_library = "lib/libdepthvistacamera.so",
)

cc_library(
    name = "depthvista_x86_64",
    hdrs = glob(["include/*.hpp"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":libdepthvistacamera",
        "@libudev",
    ],
)
