//
// Created by Cole Feuer on 2022-02-13.
//

#ifndef WEBP4J_ERRORS_H
#define WEBP4J_ERRORS_H

#include <webp/decode.h>
#include <jni.h>

void webp4j_throwInvalidInputException(JNIEnv* env);

void webp4j_throwOutOfMemoryError(JNIEnv* env);

void webp4j_throwIOException(JNIEnv* env);

void webp4j_throwIllegalArgumentException(JNIEnv* env, const char* message);

void webp4j_throwStatusCode(JNIEnv* env, VP8StatusCode statusCode);

void webp4j_throwGenericException(JNIEnv* env, const char* message);

#endif //WEBP4J_ERRORS_H
