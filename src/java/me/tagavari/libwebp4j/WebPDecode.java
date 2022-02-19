package me.tagavari.libwebp4j;

import me.tagavari.libwebp4j.NativeLoader;
import me.tagavari.libwebp4j.data.WebPBitstreamFeatures;
import me.tagavari.libwebp4j.data.WebPDecodedData;
import me.tagavari.libwebp4j.data.WebPDecoderOptions;
import me.tagavari.libwebp4j.data.WebPInfo;
import me.tagavari.libwebp4j.data.WebPColorSpace;
import me.tagavari.libwebp4j.exception.WebPException;

public class WebPDecode {
    static {
		//Make sure the native library is loaded
		NativeLoader.load();
	}

    private WebPDecode() {
        
    }
    
    public static native int getVersion();

    public static native WebPInfo getInfo(byte[] data) throws WebPException;

    public static native WebPBitstreamFeatures getFeatures(byte[] data) throws WebPException;

    public static native WebPDecodedData decodeRGBA(byte[] data) throws WebPException;
    public static native WebPDecodedData decodeARGB(byte[] data) throws WebPException;
    public static native WebPDecodedData decodeBGRA(byte[] data) throws WebPException;
    public static native WebPDecodedData decodeRGB(byte[] data) throws WebPException;
    public static native WebPDecodedData decodeBGR(byte[] data) throws WebPException;

    public static native WebPDecodedData decode(byte[] data, WebPDecoderOptions options, WebPColorSpace outputColorSpace) throws WebPException;
}