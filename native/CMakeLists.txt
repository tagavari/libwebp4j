#JAVA_HOME and JNI_GENERATED_DIR get passed from Gradle

cmake_minimum_required(VERSION 3.11)
project(webp4j C)

include(FetchContent)

set(CMAKE_C_STANDARD 17)

#Add native library
add_library(webp4j SHARED decode.c encode.c errors.c errors.h converter.c converter.h)

#Always use 'lib' prefix
set_target_properties(webp4j PROPERTIES PREFIX "lib")

#Link libwebp
find_path(WEBP_INCLUDE_DIR
    NAMES webp/decode.h
    PATHS /usr/include /usr/local/include /opt/homebrew/include
    REQUIRED
)
find_library(WEBP_LIBRARY
    NAMES webp libwebp
    HINTS /usr/lib /usr/local/lib /opt/homebrew/lib
    REQUIRED
)
target_include_directories(webp4j PRIVATE ${WEBP_INCLUDE_DIR})
target_link_libraries(webp4j ${WEBP_LIBRARY})

#Add JNI headers
target_include_directories(webp4j PRIVATE "${JAVA_HOME}/include" "${JAVA_HOME}/include/win32" "${JAVA_HOME}/include/linux" "${JAVA_HOME}/include/darwin")
INCLUDE_DIRECTORIES(${JNI_GENERATED_DIR})
