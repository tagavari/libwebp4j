//
// Created by Cole Feuer on 2022-02-13.
//

#include "errors.h"
#include <assert.h>
#include <jni.h>

#define classNameOutOfMemoryError "java/lang/OutOfMemoryError"
#define classNameIllegalArgumentException "java/lang/IllegalArgumentException"
#define classNameIOException "java/lang/IOException"
#define classNameUnsupportedOperationException "java/lang/UnsupportedOperationException"

#define classNameWebPException "me/tagavari/libwebp4j/exception/WebPException"
#define classNameWebPInvalidInputException "me/tagavari/libwebp4j/exception/WebPInvalidInputException"
#define classNameWebPNotEnoughDataException "me/tagavari/libwebp4j/exception/WebPNotEnoughDataException"

void webp4j_throwInvalidInputException(JNIEnv* env) {
	jclass errorClass = (*env)->FindClass(env, classNameWebPInvalidInputException);
	jmethodID errorConstructor = (*env)->GetMethodID(env, errorClass, "<init>", "()V");
	jobject errorObject = (*env)->NewObject(env, errorClass, errorConstructor);
	
	(*env)->Throw(env, errorObject);
}

void webp4j_throwOutOfMemoryError(JNIEnv* env) {
	jclass errorClass = (*env)->FindClass(env, classNameOutOfMemoryError);
	jmethodID errorConstructor = (*env)->GetMethodID(env, errorClass, "<init>", "()V");
	jobject errorObject = (*env)->NewObject(env, errorClass, errorConstructor);
	
	(*env)->Throw(env, errorObject);
}

void webp4j_throwIOException(JNIEnv* env) {
	jclass errorClass = (*env)->FindClass(env, classNameIOException);
	jmethodID errorConstructor = (*env)->GetMethodID(env, errorClass, "<init>", "()V");
	jobject errorObject = (*env)->NewObject(env, errorClass, errorConstructor);

	(*env)->Throw(env, errorObject);
}

void webp4j_throwIllegalArgumentException(JNIEnv* env, const char* message) {
	jobject jMessage = message != NULL ? (*env)->NewStringUTF(env, message) : NULL;

	jclass errorClass = (*env)->FindClass(env, classNameIllegalArgumentException);
	jmethodID errorConstructor = (*env)->GetMethodID(env, errorClass, "<init>", "(Ljava/lang/String;)V");
	jobject errorObject = (*env)->NewObject(env, errorClass, errorConstructor, jMessage);

	(*env)->Throw(env, errorObject);
}

void webp4j_throwStatusCode(JNIEnv* env, VP8StatusCode statusCode) {
	//Make sure we're not handling a status code of OK
	assert(statusCode != VP8_STATUS_OK);

	jobject errorObject;
	switch(statusCode) {
		case VP8_STATUS_OUT_OF_MEMORY: {
			//Throw OutOfMemoryError
			jclass errorClass = (*env)->FindClass(env, classNameOutOfMemoryError);
			jmethodID errorConstructor = (*env)->GetMethodID(env, errorClass, "<init>", "()V");
			errorObject = (*env)->NewObject(env, errorClass, errorConstructor);
			break;
		}
		case VP8_STATUS_INVALID_PARAM: {
			//Throw IllegalArgumentException
			jclass errorClass = (*env)->FindClass(env, classNameIllegalArgumentException);
			jmethodID errorConstructor = (*env)->GetMethodID(env, errorClass, "<init>", "()V");
			errorObject = (*env)->NewObject(env, errorClass, errorConstructor);
			break;
		}
		case VP8_STATUS_BITSTREAM_ERROR: {
			//Throw IOException
			jclass errorClass = (*env)->FindClass(env, classNameIOException);
			jmethodID errorConstructor = (*env)->GetMethodID(env, errorClass, "<init>", "()V");
			errorObject = (*env)->NewObject(env, errorClass, errorConstructor);
			break;
		}
		case VP8_STATUS_UNSUPPORTED_FEATURE: {
			//Throw UnsupportedOperationException
			jclass errorClass = (*env)->FindClass(env, classNameUnsupportedOperationException);
			jmethodID errorConstructor = (*env)->GetMethodID(env, errorClass, "<init>", "()V");
			errorObject = (*env)->NewObject(env, errorClass, errorConstructor);
			break;
		}
		case VP8_STATUS_SUSPENDED: {
			//Throw WebPException with description
			jclass errorClass = (*env)->FindClass(env, classNameUnsupportedOperationException);
			jmethodID errorConstructor = (*env)->GetMethodID(env, errorClass, "<init>", "(Ljava/lang/String;)V");
			errorObject = (*env)->NewObject(env, errorClass, errorConstructor, "Suspended");
			break;
		}
		case VP8_STATUS_USER_ABORT: {
			//Throw WebPException with description
			jclass errorClass = (*env)->FindClass(env, classNameUnsupportedOperationException);
			jmethodID errorConstructor = (*env)->GetMethodID(env, errorClass, "<init>", "(Ljava/lang/String;)V");
			errorObject = (*env)->NewObject(env, errorClass, errorConstructor, "User aborted");
			break;
		}
		case VP8_STATUS_NOT_ENOUGH_DATA: {
			//Throw WebPNotEnoughDataException
			jclass errorClass = (*env)->FindClass(env, classNameWebPNotEnoughDataException);
			jmethodID errorConstructor = (*env)->GetMethodID(env, errorClass, "<init>", "()V");
			errorObject = (*env)->NewObject(env, errorClass, errorConstructor);
			break;
		}
		default: {
			//Throw WebPException
			jclass errorClass = (*env)->FindClass(env, classNameWebPException);
			jmethodID errorConstructor = (*env)->GetMethodID(env, errorClass, "<init>", "()V");
			errorObject = (*env)->NewObject(env, errorClass, errorConstructor);
			break;
		}
	}

	(*env)->Throw(env, errorObject);
}

void webp4j_throwGenericException(JNIEnv* env, const char* message) {
	jclass errorClass = (*env)->FindClass(env, classNameUnsupportedOperationException);
	jmethodID errorConstructor = (*env)->GetMethodID(env, errorClass, "<init>", "(Ljava/lang/String;)V");
	jobject errorObject = (*env)->NewObject(env, errorClass, errorConstructor, message);
	(*env)->Throw(env, errorObject);
}