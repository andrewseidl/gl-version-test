#define EGL_EGLEXT_PROTOTYPES // for EGL extensions

#include <GL/gl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <stdexcept>
#include <iostream>

void checkEGLError() {
  const EGLint err = eglGetError();
  if (err != EGL_SUCCESS) {
    const char *error = NULL;
    switch (err) {
    case EGL_NOT_INITIALIZED:
      error = "NOT_INITIALIZED";
      break;
    case EGL_BAD_ACCESS:
      error = "BAD_ACCESS";
      break;
    case EGL_BAD_ALLOC:
      error = "BAD_ALLOC";
      break;
    case EGL_BAD_ATTRIBUTE:
      error = "BAD_ATTRIBUTE";
      break;
    case EGL_BAD_CONTEXT:
      error = "BAD_CONTEXT";
      break;
    case EGL_BAD_CONFIG:
      error = "BAD_CONFIG";
      break;
    case EGL_BAD_CURRENT_SURFACE:
      error = "BAD_CURRENT_SURFACE";
      break;
    case EGL_BAD_DISPLAY:
      error = "BAD_DISPLAY";
      break;
    case EGL_BAD_SURFACE:
      error = "BAD_SURFACE";
      break;
    case EGL_BAD_MATCH:
      error = "BAD_MATCH";
      break;
    case EGL_BAD_PARAMETER:
      error = "BAD_PARAMETER";
      break;
    case EGL_BAD_NATIVE_PIXMAP:
      error = "BAD_NATIVE_PIXMAP";
      break;
    case EGL_BAD_NATIVE_WINDOW:
      error = "BAD_NATIVE_WINDOW";
      break;
    case EGL_CONTEXT_LOST:
      error = "CONTEXT_LOST";
      break;
    default:
      error = "(unknown)";
      break;
    }

    throw std::runtime_error("Error EGL_" + std::string(error));
  }
}

static const EGLint configAttribs[] = {
    EGL_SURFACE_TYPE,    EGL_PBUFFER_BIT, EGL_BLUE_SIZE,  8, EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE,        8,               EGL_ALPHA_SIZE, 8,
    // EGL_DEPTH_SIZE,
    // 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,  EGL_NONE};

static const int pbufferWidth = 1024;
static const int pbufferHeight = 1024;

static const EGLint pbufferAttribs[] = {
    EGL_WIDTH, pbufferWidth, EGL_HEIGHT, pbufferHeight, EGL_NONE,
};

static const EGLint contextAttribs[] = {
    EGL_CONTEXT_MAJOR_VERSION,       4,
    EGL_CONTEXT_MINOR_VERSION,       5,
    EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
    EGL_NONE,
};

int main(int argc, char *argv[]) {
  // 1. Initialize EGL

  static const int MAX_DEVICES = 4;
  EGLDeviceEXT eglDevs[MAX_DEVICES];
  EGLint numDevices;

  PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT =
      (PFNEGLQUERYDEVICESEXTPROC)eglGetProcAddress("eglQueryDevicesEXT");
  checkEGLError();

  eglQueryDevicesEXT(MAX_DEVICES, eglDevs, &numDevices);
  checkEGLError();

  PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT =
      (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress(
          "eglGetPlatformDisplayEXT");
  checkEGLError();

  EGLDisplay eglDpy =
      eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, eglDevs[0], 0);
  checkEGLError();

  PFNEGLQUERYDEVICEATTRIBEXTPROC eglQueryDeviceAttribEXT =
      (PFNEGLQUERYDEVICEATTRIBEXTPROC)eglGetProcAddress(
          "eglQueryDeviceAttribEXT");
  checkEGLError();

  EGLint major, minor;

  eglInitialize(eglDpy, &major, &minor);
  checkEGLError();

  // 2. Select an appropriate configuration
  EGLint numConfigs;
  EGLConfig eglCfg;

  eglChooseConfig(eglDpy, configAttribs, &eglCfg, 1, &numConfigs);
  checkEGLError();

  // 3. Create a surface
  EGLSurface eglSurf = eglCreatePbufferSurface(eglDpy, eglCfg, pbufferAttribs);
  checkEGLError();

  // 4. Bind the API
  eglBindAPI(EGL_OPENGL_API);
  checkEGLError();

  // 5. Create a context and make it current
  EGLContext eglCtx =
      eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, contextAttribs);
  checkEGLError();

  eglMakeCurrent(eglDpy, eglSurf, eglSurf, eglCtx);
  checkEGLError();

  const char *glVersion = (const char *)glGetString(GL_VERSION);
  std::string glVersionStr = "undefined";
  if (glVersion) {
    glVersionStr = glVersion;
  }
  std::cout << "GL_VERSION: " << glVersionStr << std::endl;

  return 0;
}
