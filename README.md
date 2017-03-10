# gl-version-test

Simple test to verify that EGL and some NVIDIA EGL extensions can be used inside a container.

On success will output `GL_VERSION` after EGL context creation.

Bundled EGL and KHR headers are directly from Khronos.

For simplicity this repo includes a prebuilt copy of the `gl-version-test` executable.

## nvidia-docker usage

The 375 drivers series introduced glvnd, which in turn introduced a dependency on the new EGL vendor config dirs: `/etc/glvnd/egl_vendor.d` and `/usr/share/glvnd/egl_vendor.d`. As of [nvidia-docker](https://github.com/NVIDIA/nvidia-docker) 1.0.1 you need to explicitly bind mount the latter directory in order to use EGL inside a container:

    $ nvidia-docker run --rm andrewseidl/gl-version-test
    terminate called after throwing an instance of 'std::runtime_error'
      what():  Error EGL_BAD_PARAMETER
    Aborted (core dumped)

    $ nvidia-docker run --rm -v /usr/share/glvnd/egl_vendor.d:/usr/share/glvnd/egl_vendor.d andrewseidl/gl-version-test
    GL_VERSION: 4.5.0 NVIDIA 378.13
