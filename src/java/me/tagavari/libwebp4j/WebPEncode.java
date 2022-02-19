package me.tagavari.libwebp4j;

import me.tagavari.libwebp4j.NativeLoader;

public class WebPEncode {
	static {
		//Make sure the native library is loaded
		NativeLoader.load();
	}

	private WebPEncode() {
        
    }

	public static native int getVersion();

	public static native byte[] encodeRGB(byte[] data, int width, int height, int stride, float qualityFactor);
	public static native byte[] encodeBGR(byte[] data, int width, int height, int stride, float qualityFactor);
	public static native byte[] encodeRGBA(byte[] data, int width, int height, int stride, float qualityFactor);
	public static native byte[] encodeBGRA(byte[] data, int width, int height, int stride, float qualityFactor);

	public static native byte[] encodeLosslessRGB(byte[] data, int width, int height, int stride);
	public static native byte[] encodeLosslessBGR(byte[] data, int width, int height, int stride);
	public static native byte[] encodeLosslessRGBA(byte[] data, int width, int height, int stride);
	public static native byte[] encodeLosslessBGRA(byte[] data, int width, int height, int stride);
}
