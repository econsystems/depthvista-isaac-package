"""
Copyright (c) 2019-2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
"""

load("@com_nvidia_isaac_engine//bzl:cuda.bzl", "cuda_deps")

cuda_deps()

cc_library(
    name = "tensorrt",
    linkstatic = True,
    visibility = ["//visibility:public"],
    deps = select({
        "@com_nvidia_isaac_engine//engine/build:platform_x86_64": ["@tensorrt_x86_64"],
        "@com_nvidia_isaac_engine//engine/build:platform_x86_64_cuda_11_1": ["@tensorrt_x86_64_7216"],
        "@com_nvidia_isaac_engine//engine/build:platform_jetpack45": ["@tensorrt_aarch64_jetpack45"],
    }) + [
        "cublas",
    ],
)

cc_library(
    name = "libtensorflow",
    linkstatic = True,
    visibility = ["//visibility:public"],
    deps = select({
        "@com_nvidia_isaac_engine//engine/build:platform_x86_64": ["@libtensorflow_x86_64"],
        "@com_nvidia_isaac_engine//engine/build:platform_x86_64_cuda_11_1": ["@libtensorflow_x86_64_2400"],
        "@com_nvidia_isaac_engine//engine/build:platform_jetpack45": ["@libtensorflow_aarch64_jetpack45"],
    }) + [
        "//third_party:cublas",
        "//third_party:cudnn",
        "//third_party:cufft",
        "//third_party:curand",
        "//third_party:cusolver",
    ],
)

cc_library(
    name = "libtorch",
    linkstatic = True,
    visibility = ["//visibility:public"],
    deps = select({
        "@com_nvidia_isaac_engine//engine/build:platform_x86_64": ["@libtorch_x86_64"],
        "@com_nvidia_isaac_engine//engine/build:platform_x86_64_cuda_11_1": ["@libtorch_x86_64_171"],
        "@com_nvidia_isaac_engine//engine/build:platform_jetpack45": ["@libtorch_aarch64_jetpack45"],
    }) + [
        "//third_party:cublas",
        "//third_party:cuda",
        "//third_party:cudnn",
        "//third_party:cufft",
        "//third_party:curand",
        "//third_party:cusparse",
        "//third_party:nvrtc",
    ],
)

cc_library(
    name = "ros",
    visibility = ["//visibility:public"],
    deps = select({
        "@com_nvidia_isaac_engine//engine/build:cpu_host": ["@isaac_ros_bridge_x86_64"],
        "@com_nvidia_isaac_engine//engine/build:cpu_aarch64": ["@isaac_ros_bridge_aarch64_xavier"],
    }),
)

cc_library(
    name = "opencv",
    visibility = ["//visibility:public"],
    deps = select({
        "@com_nvidia_isaac_engine//engine/build:cpu_host": ["@opencv_x86_64"],
        "@com_nvidia_isaac_engine//engine/build:cpu_aarch64": ["@opencv_aarch64_jetpack45"],
    }),
)

cc_library(
    name = "libargus",
    visibility = ["//visibility:public"],
    deps = select({
        "@com_nvidia_isaac_engine//engine/build:cpu_host": [],
        "@com_nvidia_isaac_engine//engine/build:cpu_aarch64": ["@libargus_aarch64_nano"],
    }),
)

cc_library(
    name = "zed",
    visibility = ["//visibility:public"],
    deps = [":cuda"] + select({
        "@com_nvidia_isaac_engine//engine/build:platform_x86_64": [
            "@zed_x86_64",
            "@libpng12_x86_64//:libpng12_x86_64",
        ],
        "@com_nvidia_isaac_engine//engine/build:platform_x86_64_cuda_11_1": [
            "@zed_x86_64_cuda_11_1",
            "@libpng12_x86_64//:libpng12_x86_64",
        ],
        "@com_nvidia_isaac_engine//engine/build:platform_jetpack45": [
            "@zed_aarch64_jetpack45",
        ],
    }),
)

cc_library(
    name = "depthvista",
    visibility = ["//visibility:public"],
    deps = select({
        "@com_nvidia_isaac_engine//engine/build:platform_x86_64": [
            "@depthvista_x86_64",
        ],
        "@com_nvidia_isaac_engine//engine/build:platform_jetpack45": [
            "@depthvista_aarch64_jetpack45",
        ],
    })
)

cc_library(
    name = "lula",
    visibility = ["//visibility:public"],
    deps = select({
        "@com_nvidia_isaac_engine//engine/build:cpu_host": ["@lula_x86_64//:lula"],
        "@com_nvidia_isaac_engine//engine/build:cpu_aarch64": ["@lula_aarch64//:lula"],
    }),
)

cc_library(
    name = "libnvrc",
    visibility = ["//visibility:public"],
    deps = select({
        "@com_nvidia_isaac_engine//engine/build:cpu_host": ["@libnvrc_x86"],
        "@com_nvidia_isaac_engine//engine/build:cpu_aarch64": ["@libnvrc_aarch64"],
    }),
)
