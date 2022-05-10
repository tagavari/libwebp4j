package me.tagavari.libwebp4j.data;

public class WebPDecodedData {
	private final byte[] bytes;
	private final int width, height;

	WebPDecodedData(byte[] bytes, int width, int height) {
		this.bytes = bytes;
		this.width = width;
		this.height = height;
	}

	public byte[] getBytes() {
		return bytes;
	}

	public int getWidth() {
		return width;
	}

	public int getHeight() {
		return height;
	}
}
