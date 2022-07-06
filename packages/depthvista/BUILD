load("@com_nvidia_isaac_sdk//bzl:module.bzl", "isaac_cc_module", "isaac_component")

isaac_component(
  name = "depthvista_camera",
  visibility = ["//visibility:public"],
  deps = [
   		"//third_party:depthvista",
        "@com_nvidia_isaac_engine//engine/gems/geometry:pinhole",
        "@com_nvidia_isaac_engine//engine/gems/system:cuda_context",
    ],
)

isaac_cc_module(
  name = "depthvista",
  deps = [
	":depthvista_camera",
  ],
)
