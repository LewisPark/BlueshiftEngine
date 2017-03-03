set(ANDROID TRUE)

set(CMAKE_SYSTEM_NAME Android)

set(CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION clang)
set(CMAKE_ANDROID_API_MIN 21)
set(CMAKE_ANDROID_API 21)
set(CMAKE_ANDROID_NDK $ENV{ANDROID_HOME})
set(CMAKE_ANDROID_SKIP_ANT_STEP 0)
set(CMAKE_ANDROID_JAVA_SOURCE_DIR java)
set(CMAKE_ANDROID_BUILD_SYSTEM GradleBuild)
set(CMAKE_ANDROID_STL_TYPE gnustl_static)
