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
        sha256 = "bb1976e089c6adcc3d5ae9642d9f1853c2a7ae2b953c969e64dbd5daf4cd2165",
		url = "https://www.dropbox.com/s/94zog158ei78cmp/libdepthvistacamera_x86_64.tar.xz?dl=1",
        type = "tar.xz",
        licenses = [],
    )

    isaac_http_archive(
        name = "depthvista_aarch64_jetpack45",
        build_file = clean_dep("//third_party:depthvista_jetpack45.BUILD"),
        sha256 = "22f4be36177e216b59b1595bbf965bea822f51c1125f3d09e12e3670604a23c9",
        url = "https://www.dropbox.com/s/lgl8jx7f2bzibfc/libdepthvistacamera_jetpack45.tar.xz?dl=1",
        type = "tar.xz",
        licenses = [],
    )
