//
// Created by Cole Feuer on 2022-02-13.
//

#ifndef WEBP4J_CONVERTER_H
#define WEBP4J_CONVERTER_H

#include <webp/decode.h>
#include <jni.h>

void applyWebPDecoderConfig(JNIEnv* env, jobject jOptions, WebPDecoderConfig* outConfig);

jobject createDecodedData(JNIEnv* env, void* data, int dataLen, int width, int height);

WEBP_CSP_MODE importWebPColorSpace(JNIEnv* env, jobject jColorSpace);
jobject exportWebPColorSpace(JNIEnv* env, WEBP_CSP_MODE colorSpace);

#endif //WEBP4J_CONVERTER_H
