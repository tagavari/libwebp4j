#include <webp/encode.h>
#include <jni.h>
#include <me_tagavari_libwebp4j_WebPEncode.h>
#include "errors.h"

JNIEXPORT jint JNICALL Java_me_tagavari_libwebp4j_WebPEncode_getVersion(JNIEnv* env, jclass class) {
	return WebPGetDecoderVersion();
}

typedef size_t (*WEBP_SIMPLE_ENCODE)(const uint8_t* data, int width, int height, int stride, float qualityFactor, uint8_t** output);
static jbyteArray encodeSimple(JNIEnv* env, jbyteArray jData, jint width, jint height, jint stride, jfloat qualityFactor, WEBP_SIMPLE_ENCODE encode, int bytesPerPixel) {
	//Validate the input
	if(width <= 0) {
		webp4j_throwIllegalArgumentException(env, "Invalid width provided");
		return NULL;
	}
	if(height <= 0) {
		webp4j_throwIllegalArgumentException(env, "Invalid height provided");
		return NULL;
	}
	if(qualityFactor < 0 || qualityFactor > 100) {
		webp4j_throwIllegalArgumentException(env, "Invalid quality factor provided");
		return NULL;
	}

	int inputDataLength = (*env)->GetArrayLength(env, jData);
	if(inputDataLength < width * height * bytesPerPixel) {
		webp4j_throwIllegalArgumentException(env, "Not enough input data provided");
		return NULL;
	}

	//Read the data
	void* inputData = (*env)->GetByteArrayElements(env, jData, NULL);

	//Run the operation
	void* outputData;
	int outputDataLength = (int) encode(inputData, width, height, stride, qualityFactor, (uint8_t**) &outputData);

	//Release the data, ignoring any changes (we don't modify the input array)
	(*env)->ReleaseByteArrayElements(env, jData, inputData, JNI_ABORT);

	if(outputDataLength != 0) {
		//Return the result
		jbyteArray result = (*env)->NewByteArray(env, outputDataLength);
		(*env)->SetByteArrayRegion(env, result, 0, outputDataLength, outputData);
		WebPFree(outputData);
		return result;
	} else {
		//Throw an error
		webp4j_throwIOException(env);
		return NULL;
	}
}

JNIEXPORT jbyteArray JNICALL Java_me_tagavari_libwebp4j_WebPEncode_encodeRGB(JNIEnv* env, jclass class, jbyteArray jData, jint width, jint height, jint stride, jfloat qualityFactor) {
	return encodeSimple(env, jData, width, height, stride, qualityFactor, WebPEncodeRGB, 3);
}
JNIEXPORT jbyteArray JNICALL Java_me_tagavari_libwebp4j_WebPEncode_encodeBGR(JNIEnv* env, jclass class, jbyteArray jData, jint width, jint height, jint stride, jfloat qualityFactor) {
	return encodeSimple(env, jData, width, height, stride, qualityFactor, WebPEncodeBGR, 3);
}
JNIEXPORT jbyteArray JNICALL Java_me_tagavari_libwebp4j_WebPEncode_encodeRGBA(JNIEnv* env, jclass class, jbyteArray jData, jint width, jint height, jint stride, jfloat qualityFactor) {
	return encodeSimple(env, jData, width, height, stride, qualityFactor, WebPEncodeRGBA, 4);
}
JNIEXPORT jbyteArray JNICALL Java_me_tagavari_libwebp4j_WebPEncode_encodeBGRA(JNIEnv* env, jclass class, jbyteArray jData, jint width, jint height, jint stride, jfloat qualityFactor) {
	return encodeSimple(env, jData, width, height, stride, qualityFactor, WebPEncodeBGRA, 4);
}

typedef size_t (*WEBP_SIMPLE_LOSSLESS_ENCODE)(const uint8_t* data, int width, int height, int stride, uint8_t** output);
static jbyteArray encodeSimpleLossless(JNIEnv* env, jbyteArray jData, jint width, jint height, jint stride, WEBP_SIMPLE_LOSSLESS_ENCODE encode, int bytesPerPixel) {
	//Validate the input
	if(width <= 0) {
		webp4j_throwIllegalArgumentException(env, "Invalid width provided");
		return NULL;
	}
	if(height <= 0) {
		webp4j_throwIllegalArgumentException(env, "Invalid height provided");
		return NULL;
	}

	int inputDataLength = (*env)->GetArrayLength(env, jData);
	if(inputDataLength < width * height * bytesPerPixel) {
		webp4j_throwIllegalArgumentException(env, "Not enough input data provided");
		return NULL;
	}

	//Read the data
	void* inputData = (*env)->GetByteArrayElements(env, jData, NULL);

	//Run the operation
	void* outputData;
	int outputDataLength = (int) encode(inputData, width, height, stride, (uint8_t**) &outputData);

	//Release the data, ignoring any changes (we don't modify the input array)
	(*env)->ReleaseByteArrayElements(env, jData, inputData, JNI_ABORT);

	if(outputDataLength != 0) {
		//Return the result
		jbyteArray result = (*env)->NewByteArray(env, outputDataLength);
		(*env)->SetByteArrayRegion(env, result, 0, outputDataLength, outputData);
		WebPFree(outputData);
		return result;
	} else {
		//Throw an error
		webp4j_throwIOException(env);
		return NULL;
	}
}

JNIEXPORT jbyteArray JNICALL Java_me_tagavari_libwebp4j_WebPEncode_encodeLosslessRGB(JNIEnv* env, jclass class, jbyteArray jData, jint width, jint height, jint stride) {
	return encodeSimpleLossless(env, jData, width, height, stride, WebPEncodeLosslessRGB, 3);
}
JNIEXPORT jbyteArray JNICALL Java_me_tagavari_libwebp4j_WebPEncode_encodeLosslessBGR(JNIEnv* env, jclass class, jbyteArray jData, jint width, jint height, jint stride) {
	return encodeSimpleLossless(env, jData, width, height, stride, WebPEncodeLosslessBGR, 3);
}
JNIEXPORT jbyteArray JNICALL Java_me_tagavari_libwebp4j_WebPEncode_encodeLosslessRGBA(JNIEnv* env, jclass class, jbyteArray jData, jint width, jint height, jint stride) {
	return encodeSimpleLossless(env, jData, width, height, stride, WebPEncodeLosslessRGBA, 4);
}
JNIEXPORT jbyteArray JNICALL Java_me_tagavari_libwebp4j_WebPEncode_encodeLosslessBGRA(JNIEnv* env, jclass class, jbyteArray jData, jint width, jint height, jint stride) {
	return encodeSimpleLossless(env, jData, width, height, stride, WebPEncodeLosslessBGRA, 4);
}