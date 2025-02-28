#include <jni.h>
#include <android/native_window_jni.h>
#include <camera/NdkCameraManager.h>
#include <media/NdkImageReader.h>
#include <android/log.h>
#include <dlfcn.h>

#define LOG_TAG "Native"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGR(...) __android_log_print(ANDROID_LOG_ERROR, "RustLib", __VA_ARGS__)

ANativeWindow *nativeWindow = nullptr;
ACameraDevice *cameraDevice = nullptr;
ACaptureSessionOutput *sessionOutput = nullptr;
ACaptureSessionOutputContainer *outputContainer = nullptr;
ACameraCaptureSession *captureSession = nullptr;
ACameraOutputTarget *cameraOutputTarget = nullptr;
AImageReader *imageReader = nullptr;
ACameraManager *cameraManager = nullptr;

// Callback to process frames
void imageCallback(void *context, AImageReader *reader) {
  AImage *image = nullptr;
  if (AImageReader_acquireLatestImage(reader, &image) == AMEDIA_OK) {
    LOGE("New frame captured");

    // Get image data (Example: YUV_420)
    uint8_t *data = nullptr;
    int dataLength = 0;
    AImage_getPlaneData(image, 0, &data, &dataLength);

    if (data) {
      LOGE("Frame size: %d", dataLength);
    }

    // Close image when done
    AImage_delete(image);
  }
}

// Set up Surface
extern "C"
JNIEXPORT void JNICALL
Java_com_example_android_1native_1cpp_MainActivity_setSurfaceJNI(JNIEnv *env, jobject, jobject surface) {
  if (!surface) {
    LOGE("ERROR: Surface is NULL!");
    return;
  }

  nativeWindow = ANativeWindow_fromSurface(env, surface);
  LOGE("Surface received.");
}

// Open camera and start streaming
extern "C"
JNIEXPORT void JNICALL
Java_com_example_android_1native_1cpp_MainActivity_openCameraJNI(JNIEnv *env, jobject) {
  LOGE("Opening Camera...");
  cameraManager = ACameraManager_create();

  ACameraIdList *cameraIdList = nullptr;
  ACameraManager_getCameraIdList(cameraManager, &cameraIdList);

  if (cameraIdList->numCameras > 0) {
    const char *cameraId = cameraIdList->cameraIds[0];

    ACameraDevice_StateCallbacks cameraDeviceCallbacks = {};
    cameraDeviceCallbacks.onDisconnected = [](void *, ACameraDevice *device) {
      ACameraDevice_close(device);
    };

    if (ACameraManager_openCamera(cameraManager, cameraId, &cameraDeviceCallbacks, &cameraDevice) != ACAMERA_OK) {
      LOGE("Failed to open camera.");
      return;
    }
    LOGE("Camera opened: %s", cameraId);
  }

  // Setup AImageReader for capturing frames
  AImageReader_new(640, 480, AIMAGE_FORMAT_YUV_420_888, 4, &imageReader);
  AImageReader_setImageListener(imageReader, new AImageReader_ImageListener{nullptr, imageCallback});
LOGE("ImageReader setup complete.");
  ACameraManager_deleteCameraIdList(cameraIdList);
}

extern "C" {
  jint add_numbers(jint a, jint b);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_android_1native_1cpp_MainActivity_addNumbersJNI(JNIEnv *env, jobject, jint a, jint b) {
  return add_numbers(a, b);
}
// typedef jint (*AddNumbersFn)(jint, jint);
// extern "C"
// JNIEXPORT jint JNICALL
// Java_com_example_android_1native_1cpp_MainActivity_addNumbersJNI(JNIEnv *env, jobject, jint a, jint b) {
//     void* handle = dlopen("libandroid_rust_lib.so", RTLD_NOW);
//     if (!handle) {
//         LOGE("❌ dlopen failed: %s", dlerror());
//         return -1;
//     }
//
//     LOGE("✅ Rust library loaded!");
//
//     AddNumbersFn add_numbers = (AddNumbersFn)dlsym(handle, "add_numbers");
//     if (!add_numbers) {
//         LOGE("❌ dlsym failed: %s", dlerror());
//         return -2;
//     }
//
//     LOGE("✅ Rust function found! Calling...");
//     jint result = add_numbers(a, b);
//     LOGE("✅ Rust function returned: %d", result);
//
//     dlclose(handle);
//     return result;
// }
