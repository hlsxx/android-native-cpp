cmake_minimum_required(VERSION 3.22.1)
project("android_native_cpp")

# Define Rust library paths
set(RUST_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
set(RUST_LIB_PATH ${RUST_LIB_DIR}/libandroid_rust_lib.so)

# Check if Rust library exists
if(NOT EXISTS ${RUST_LIB_PATH})
    message(WARNING "⚠️ Rust library not found at: ${RUST_LIB_PATH}")
endif()

# Add Rust shared library (always add to avoid linker errors)
add_library(android_rust_lib SHARED IMPORTED)
set_target_properties(android_rust_lib PROPERTIES IMPORTED_LOCATION ${RUST_LIB_PATH})

# Add main native C++ library
add_library(${CMAKE_PROJECT_NAME} SHARED native-lib.cpp)

# Find system libraries
find_library(camera2ndk-lib camera2ndk)
find_library(media-lib mediandk)

if(NOT camera2ndk-lib OR NOT media-lib)
    message(WARNING "⚠️ Some system libraries (camera2ndk, mediandk) were not found!")
endif()

# Link libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        ${camera2ndk-lib}
        ${media-lib}
        android_rust_lib
)
