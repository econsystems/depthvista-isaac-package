load("@com_nvidia_isaac_engine//bzl:deps.bzl", "isaac_http_archive", "isaac_new_local_repository")

def clean_dep(dep):
    return str(Label(dep))

# loads dependencies for depthvista tof camera
def isaac_depthvista_workspace():
    isaac_new_local_repository(
        name = "libudev",
        build_file = clean_dep("//third_party:libudev.BUILD"),
        path = "/lib",
        licenses = ["https://raw.githubusercontent.com/systemd/systemd/master/LICENSE.LGPL2.1"],
    )

    isaac_http_archive(
        name = "libudev_aarch64",
        build_file = clean_dep("//third_party:libudev_aarch64.BUILD"),
        sha256 = "19634360f2b305d4d4ea883650c8bb6f1622d0f129d807274354fe7fc4d4eb33",
        url = "https://developer.nvidia.com/isaac/download/third_party/libudev-aarch64-tar-xz",
        type = "tar.xz",
        licenses = ["https://raw.githubusercontent.com/systemd/systemd/master/LICENSE.LGPL2.1"],
    )

    isaac_http_archive(
        name = "depthvista_x86_64",
        build_file = clean_dep("//third_party:depthvista_x86_64.BUILD"),
        sha256 = "83bdcbe44b4c453ac95733157426235863cd38f4b064f1722e8fdaa034377a22",
		url = "https://www.dropbox.com/s/o3k78py2jzlcf5j/libdepthvistacamera_x86_64.tar.xz?dl=1",
        type = "tar.xz",
        licenses = [],
    )

    isaac_http_archive(
        name = "depthvista_aarch64_jetpack45",
        build_file = clean_dep("//third_party:depthvista_jetpack45.BUILD"),
        sha256 = "28b41cacfd88d9dc3b860d59b24db9387809b8e27d4884e7aca2a4182a2170c9",
        url = "https://www.dropbox.com/s/10kxs97h37t7u2b/libdepthvistacamera_jetpack45.tar.xz?dl=1",
        type = "tar.xz",
        licenses = [],
    )
