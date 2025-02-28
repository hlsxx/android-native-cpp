#include <jni.h>
#include <camera/NdkCameraManager.h>
#include <string>
#include <android/log.h>

#define LOG_TAG "Native"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_android_1native_1cpp_MainActivity_stringFromJNI(
  JNIEnv* env,
  jobject
) {
  std::string hello = "Hello from C++";
  return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_android_1native_1cpp_MainActivity_openCameraJNI(
  JNIEnv* env,
  jobject
) {
  ACameraManager* cameraManager = ACameraManager_create();
  if (!cameraManager) {
    return env->NewStringUTF("Failed to create Camera Manager");
  }

  ACameraIdList* cameraIdList = nullptr;
  ACameraManager_getCameraIdList(cameraManager, &cameraIdList);
  if (!cameraIdList || cameraIdList->numCameras < 1) {
    return env->NewStringUTF("No camera found");
  }

  const char* cameraId = cameraIdList->cameraIds[0];
  LOGD("Opening camera: %s", cameraId);

  // Release resources
  ACameraManager_deleteCameraIdList(cameraIdList);
  ACameraManager_delete(cameraManager);

  return env->NewStringUTF("Camera opened successfully");
}
