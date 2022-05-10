#JAVA_HOME and JNI_GENERATED_DIR get passed from Gradle

cmake_minimum_required(VERSION 3.11)
project(webp4j C)

include(FetchContent)

set(CMAKE_C_STANDARD 17)

#Add native library
add_library(webp4j SHARED decode.c encode.c errors.c errors.h converter.c converter.h)

#Link libwebp
#FetchContent_Declare(
#		webp
#		GIT_REPOSITORY https://chromium.googlesource.com/webm/libwebp
#		GIT_TAG        1.2.2
#		GIT_SHALLOW    TRUE
#)
#FetchContent_MakeAvailable(webp)
#target_link_libraries(webp4j webp)

#Add homebrew search paths for Apple Silicon
if(APPLE AND CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
    target_link_directories(webp4j PRIVATE "/opt/homebrew/lib")
    target_include_directories(webp4j PRIVATE "/opt/homebrew/include")
endif()

target_link_libraries(webp4j webp)

#Add JNI headers
target_include_directories(webp4j PRIVATE "${JAVA_HOME}/include" "${JAVA_HOME}/include/win32" "${JAVA_HOME}/include/linux" "${JAVA_HOME}/include/darwin")
INCLUDE_DIRECTORIES(${JNI_GENERATED_DIR})
