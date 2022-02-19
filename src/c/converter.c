//
// Created by Cole Feuer on 2022-02-13.
//

#include "converter.h"
#include <webp/decode.h>

#define classNameWebPColorSpace "me/tagavari/libwebp4j/data/WebPColorSpace"
#define classNameWebPDecodedData "me/tagavari/libwebp4j/data/WebPDecodedData"

#define loadEnumValue(env, jEnumClass, classPath, value) (*env)->GetStaticObjectField(env, jEnumClass, (*env)->GetStaticFieldID(env, jEnumClass, value, "L" classPath ";"))

void applyWebPDecoderConfig(JNIEnv* env, jobject jOptions, WebPDecoderConfig* config) {
	jclass jClassDecoderOptions = (*env)->GetObjectClass(env, jOptions);
	config->options.bypass_filtering = (*env)->CallBooleanMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "bypassFiltering", "()Z"));
	config->options.no_fancy_upsampling = (*env)->CallBooleanMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "noFancyUpscaling", "()Z"));
	config->options.use_cropping = (*env)->CallBooleanMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "useCropping", "()Z"));
	config->options.crop_left = (*env)->CallIntMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "getCropLeft", "()I"));
	config->options.crop_top = (*env)->CallIntMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "getCropTop", "()I"));
	config->options.crop_width = (*env)->CallIntMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "getCropWidth", "()I"));
	config->options.crop_height = (*env)->CallIntMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "getCropHeight", "()I"));
	config->options.use_scaling = (*env)->CallBooleanMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "useScaling", "()Z"));
	config->options.scaled_width = (*env)->CallIntMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "getScaledWidth", "()I"));
	config->options.scaled_height = (*env)->CallIntMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "getScaledHeight", "()I"));
	config->options.use_threads = (*env)->CallBooleanMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "useThreads", "()Z"));
	config->options.dithering_strength = (*env)->CallIntMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "getDitheringStrength", "()I"));
	config->options.flip = (*env)->CallBooleanMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "doFlip", "()Z"));
	config->options.alpha_dithering_strength = (*env)->CallIntMethod(env, jOptions, (*env)->GetMethodID(env, jClassDecoderOptions, "getAlphaDitheringStrength", "()I"));
}

jobject createDecodedData(JNIEnv* env, void* data, int dataLen, int width, int height) {
	//Create the byte array
	jbyteArray jData = (*env)->NewByteArray(env, dataLen);
	(*env)->SetByteArrayRegion(env, jData, 0, dataLen, data);

	jclass class = (*env)->FindClass(env, classNameWebPDecodedData);
	jmethodID constructor = (*env)->GetMethodID(env, class, "<init>", "([BII)V");
	jobject object = (*env)->NewObject(env, class, constructor, jData, width, height);

	return object;
}

struct JavaEnumMap {
	const char* javaName;
	int cValue;
};

#define DEFINE_MAPPING_FUNCTIONS(NAME_IMPORT, NAME_EXPORT, C_TYPE, VALUE_CLASS, ...)									\
	C_TYPE NAME_IMPORT(JNIEnv* env, jobject jObject) {																	\
		jclass jEnumClass = (*env)->FindClass(env, VALUE_CLASS);														\
																														\
		static const struct JavaEnumMap map[] = __VA_ARGS__;															\
		static const int mapCount = sizeof(map) / sizeof(struct JavaEnumMap);											\
																														\
		for(int i = 0; i < mapCount; ++i) {																				\
			const struct JavaEnumMap* mapValue = &map[i];																\
			jfieldID jEnumField = (*env)->GetStaticFieldID(env, jEnumClass, mapValue->javaName, "L" VALUE_CLASS ";");	\
			jobject jEnumObject = (*env)->GetStaticObjectField(env, jEnumClass, jEnumField);							\
			if((*env)->IsSameObject(env, jEnumObject, jObject)) {														\
				return mapValue->cValue;																				\
			}																											\
		}																												\
																														\
		return -1;																										\
	}																													\
																														\
	jobject NAME_EXPORT(JNIEnv* env, C_TYPE cValue) {																	\
		jclass jEnumClass = (*env)->FindClass(env, VALUE_CLASS);														\
																														\
		static const struct JavaEnumMap map[] = __VA_ARGS__;															\
		static const int mapCount = sizeof(map) / sizeof(struct JavaEnumMap);											\
																														\
		for(int i = 0; i < mapCount; ++i) {																				\
			const struct JavaEnumMap* mapValue = &map[i];																\
			if(mapValue->cValue != cValue) break;																		\
																														\
			jfieldID jEnumField = (*env)->GetStaticFieldID(env, jEnumClass, mapValue->javaName, "L" VALUE_CLASS ";");	\
			jobject jEnumObject = (*env)->GetStaticObjectField(env, jEnumClass, jEnumField);							\
			return jEnumObject;																							\
		}																												\
																														\
		return NULL;																									\
	}

DEFINE_MAPPING_FUNCTIONS(importWebPColorSpace, exportWebPColorSpace, WEBP_CSP_MODE, "me/tagavari/libwebp4j/data/WebPColorSpace", {
	{"RGB", MODE_RGB},
	{"RGBA", MODE_RGBA},
	{"BGR", MODE_BGR},
	{"BGRA", MODE_BGRA},
	{"ARGB", MODE_ARGB},
	{"RGBA_4444", MODE_RGBA_4444},
	{"RGB_565", MODE_RGB_565},
	{"rgbA", MODE_rgbA},
	{"bgrA", MODE_bgrA},
	{"Argb", MODE_Argb},
	{"rgbA_4444", MODE_rgbA_4444},
	{"YUV", MODE_YUV},
	{"YUVA", MODE_YUVA}
})