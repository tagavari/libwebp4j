#include <stdbool.h>
#include <stdlib.h>
#include <webp/decode.h>
#include <jni.h>
#include <me_tagavari_libwebp4j_WebPDecode.h>
#include "errors.h"
#include "converter.h"

#define classNameWebPInfo "me/tagavari/libwebp4j/data/WebPInfo"
#define classNameWebPBitstreamFeatures "me/tagavari/libwebp4j/data/WebPBitstreamFeatures"
#define classNameWebPBitstreamFeaturesFormat "me/tagavari/libwebp4j/data/WebPBitstreamFeatures$Format"

// getBytesPerPixel returns the number of bytes per pixel for the specified colorspace
static int getBytesPerPixel(WEBP_CSP_MODE colorspace) {
	switch(colorspace) {
		case MODE_RGBA_4444:
		case MODE_RGB_565:
		case MODE_rgbA_4444:
			return 2;
		case MODE_RGB:
		case MODE_BGR:
		case MODE_YUV:
			return 3;	
		default:
			return 4;
	}
}

// getBufferSize gets the buffer size required to hold the decoded pixels for the specified colorspace and dimensions
static int getBufferSize(WEBP_CSP_MODE colorspace, int width, int height) {
	int bytesPerPixel = getBytesPerPixel(colorspace);
	int pixelCount = width * height;
	int outputBufferLen = pixelCount * bytesPerPixel;

	return outputBufferLen;
}

JNIEXPORT jint JNICALL Java_me_tagavari_libwebp4j_WebPDecode_getVersion(JNIEnv* env, jclass class) {
	return WebPGetDecoderVersion();
}

JNIEXPORT jobject JNICALL Java_me_tagavari_libwebp4j_WebPDecode_getInfo(JNIEnv* env, jclass class, jbyteArray jData) {
	//Read the data
	int dataLength = (*env)->GetArrayLength(env, jData);
	void* data = (*env)->GetByteArrayElements(env, jData, NULL);
	
	//Run the operation
	int width, height;
	bool result = WebPGetInfo(data, dataLength, &width, &height);
	
	//Release the data, ignoring any changes (we don't modify the input array)
	(*env)->ReleaseByteArrayElements(env, jData, data, JNI_ABORT);
	
	if(result) {
		//Create the result object
		jclass jClassWebPInfo = (*env)->FindClass(env, classNameWebPInfo);
		jmethodID jMethodWebPInfoCtor = (*env)->GetMethodID(env, jClassWebPInfo, "<init>","(II)V");
		jobject jObjectWebPInfo = (*env)->NewObject(env, jClassWebPInfo, jMethodWebPInfoCtor, width, height);
		
		return jObjectWebPInfo;
	} else {
		//Throw an exception
		webp4j_throwInvalidInputException(env);
		return NULL;
	}
}

JNIEXPORT jobject JNICALL Java_me_tagavari_libwebp4j_WebPDecode_getFeatures(JNIEnv* env, jclass class, jbyteArray jData) {
	//Read the data
	int dataLength = (*env)->GetArrayLength(env, jData);
	void* data = (*env)->GetByteArrayElements(env, jData, NULL);
	
	//Run the operation
	WebPBitstreamFeatures features;
	VP8StatusCode resultCode = WebPGetFeatures(data, dataLength, &features);
	
	//Release the data, ignoring any changes (we don't modify the input array)
	(*env)->ReleaseByteArrayElements(env, jData, data, JNI_ABORT);
	
	if(resultCode == VP8_STATUS_OK) {
		//Map the format value
		jclass jClassFormat = (*env)->FindClass(env, classNameWebPBitstreamFeaturesFormat);
		jfieldID jFormatField;
		switch(features.format) {
			case 0:
				jFormatField = (*env)->GetStaticFieldID(env, jClassFormat, "UNDEFINED", "L" classNameWebPBitstreamFeaturesFormat ";");
				break;
			case 1:
				jFormatField = (*env)->GetStaticFieldID(env, jClassFormat, "LOSSY", "L" classNameWebPBitstreamFeaturesFormat ";");
				break;
			case 2:
				jFormatField = (*env)->GetStaticFieldID(env, jClassFormat, "LOSSLESS", "L" classNameWebPBitstreamFeaturesFormat ";");
				break;
		}
		jobject jFormat = (*env)->GetStaticObjectField(env, jClassFormat, jFormatField);
		
		//Create the result object
		jclass jClassResult = (*env)->FindClass(env, classNameWebPBitstreamFeatures);
		jmethodID jMethodResultCtor = (*env)->GetMethodID(env, jClassResult, "<init>", "(IIZZL" classNameWebPBitstreamFeaturesFormat ";)V");
		jobject jObjectResult = (*env)->NewObject(env, jClassResult, jMethodResultCtor, features.width, features.height, features.has_alpha, features.has_animation, jFormat);
		return jObjectResult;
	} else {
		//Throw an exception
		webp4j_throwStatusCode(env, resultCode);

		return NULL;
	}
}

typedef uint8_t* (*WEBP_SIMPLE_DECODE)(const uint8_t* data, size_t data_size, int* width, int* height);

static jobject decodeSimple(JNIEnv* env, jbyteArray jData, WEBP_SIMPLE_DECODE decode, WEBP_CSP_MODE colorspace) {
	//Read the data
	int inputDataLength = (*env)->GetArrayLength(env, jData);
	void* inputData = (*env)->GetByteArrayElements(env, jData, NULL);

	//Run the operation
	int width, height;
	void* outputData = decode(inputData, inputDataLength, &width, &height);

	//Check the output
	if(outputData == NULL) {
		webp4j_throwInvalidInputException(env);
		return NULL;
	}

	//Get the output data length
	int outputDataLength = getBufferSize(colorspace, width, height);

	//Release the data, ignoring any changes (we don't modify the input array)
	(*env)->ReleaseByteArrayElements(env, jData, inputData, JNI_ABORT);
	
	//Return the result
	jobject result = createDecodedData(env, outputData, outputDataLength, width, height);
	WebPFree(outputData);
	return result;
}

JNIEXPORT jobject JNICALL Java_me_tagavari_libwebp4j_WebPDecode_decodeRGBA(JNIEnv* env, jclass class, jbyteArray jData) {
	return decodeSimple(env, jData, WebPDecodeRGBA, MODE_RGBA);
}
JNIEXPORT jobject JNICALL Java_me_tagavari_libwebp4j_WebPDecode_decodeARGB(JNIEnv* env, jclass class, jbyteArray jData) {
	return decodeSimple(env, jData, WebPDecodeARGB, MODE_ARGB);
}
JNIEXPORT jobject JNICALL Java_me_tagavari_libwebp4j_WebPDecode_decodeBGRA(JNIEnv* env, jclass class, jbyteArray jData) {
	return decodeSimple(env, jData, WebPDecodeARGB, MODE_BGRA);
}
JNIEXPORT jobject JNICALL Java_me_tagavari_libwebp4j_WebPDecode_decodeRGB(JNIEnv* env, jclass class, jbyteArray jData) {
	return decodeSimple(env, jData, WebPDecodeARGB, MODE_RGB);
}
JNIEXPORT jobject JNICALL Java_me_tagavari_libwebp4j_WebPDecode_decodeBGR(JNIEnv* env, jclass class, jbyteArray jData) {
	return decodeSimple(env, jData, WebPDecodeARGB, MODE_BGR);
}

JNIEXPORT jobject JNICALL Java_me_tagavari_libwebp4j_WebPDecode_decode(JNIEnv* env, jclass class, jbyteArray jData, jobject jOptions, jobject jColorSpace) {
	//Initialize the decoder
	WebPDecoderConfig config;
	if(!WebPInitDecoderConfig(&config)) {
		//Throw an exception
		webp4j_throwGenericException(env, "Failed to initialize decoder config");
		return NULL;
	}

	//Apply the decoder options
	applyWebPDecoderConfig(env, jOptions, &config);

	//Read the input data
	int dataLen = (*env)->GetArrayLength(env, jData);
	void* data = (*env)->GetByteArrayElements(env, jData, NULL);
	
	//Get the image features
	VP8StatusCode getFeaturesResult = WebPGetFeatures(data, dataLen, &config.input);
	if(getFeaturesResult != VP8_STATUS_OK) {
		//Release the data, ignoring any changes (we don't modify the input array)
		(*env)->ReleaseByteArrayElements(env, jData, data, JNI_ABORT);

		//Throw an exception
		webp4j_throwStatusCode(env, getFeaturesResult);
		return NULL;
	}

	WEBP_CSP_MODE colorspace = importWebPColorSpace(env, jColorSpace);

	//Allocate output memory
	int bytesPerPixel = getBytesPerPixel(colorspace);
	int outputDataLength = bytesPerPixel * config.input.width * config.input.height;
	void* outputData = malloc(outputDataLength);
	if(outputData == NULL) {
		//Release the data, ignoring any changes (we don't modify the input array)
		(*env)->ReleaseByteArrayElements(env, jData, data, JNI_ABORT);

		//Throw an exception
		webp4j_throwOutOfMemoryError(env);
		return NULL;
	}
	
	config.output.colorspace = colorspace;
	config.output.u.RGBA.rgba = outputData;
	config.output.u.RGBA.size = outputDataLength;
	config.output.u.RGBA.stride = config.input.width * bytesPerPixel;
	config.output.is_external_memory = true;

	//Perform the decode operation
	VP8StatusCode decodeResult = WebPDecode(data, dataLen, &config);

	//Release input memory
	(*env)->ReleaseByteArrayElements(env, jData, data, JNI_ABORT);
	
	if(decodeResult == VP8_STATUS_OK) {
		//Return the result
		jobject result = createDecodedData(env, outputData, outputDataLength, config.output.width, config.output.height);
		free(outputData);
		return result;
	} else {
		free(outputData);

		//Throw an exception
		webp4j_throwStatusCode(env, decodeResult);
		return NULL;
	}
}