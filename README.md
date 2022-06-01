# libwebp4j

A simple JNI wrapper for [libwebp](https://developers.google.com/speed/webp/docs/api).

Feature support:
- [x] Simple decoding API
- [x] Advanced decoding API (RGBA output only)
- [x] Simple encoding API
- [ ] Advanced encoding API

All functions are implemented under the `WebPDecode` and `WebPEncode` static classes.

## Building

You will need JDK 9+, [Gradle](https://gradle.org), [CMake](https://cmake.org/), and [libwebp](https://developers.google.com/speed/webp/download) installed on your system.

To build, run `./gradlew build`. The library will be built to `build/libs/`.

Since this library uses native code, the produced archive is platform-specific.

## Examples

Example to read an input file with ImageIO, and encode it to WebP:
```java
BufferedImage image = ImageIO.read(new File("input.jpeg"));

byte[] imageBytes = new byte[image.getWidth() * image.getHeight() * 3];
int imageBytesIndex = 0;
for(int y = 0; y < image.getHeight(); y++) {
	for(int x = 0; x < image.getWidth(); x++) {
		int rgb = image.getRGB(x, y);
		imageBytes[imageBytesIndex++] = (byte) (rgb >> 16 & 255); //R
		imageBytes[imageBytesIndex++] = (byte) (rgb >> 8 & 255); //G
		imageBytes[imageBytesIndex++] = (byte) (rgb & 255); //B
	}
}

byte[] encodedImageBytes = WebPEncode.encodeRGB(imageBytes, image.getWidth(), image.getHeight(), image.getWidth() * 3, 80);
Files.write(Path.of("output.webp"), encodedImageBytes);
```

Example to decode a WebP file with the advanced decoding API, flip it, and re-encode it at low quality:
```java
byte[] originalImageBytes = Files.readAllBytes(Path.of("input.webp"));

WebPDecodedData data = WebPDecode.decode(originalImageBytes,
	new WebPDecoderOptions.Builder()
		.setFlip(true)
		.build(),
	WebPColorSpace.RGBA);

byte[] newImageBytes = WebPEncode.encodeRGBA(data.getBytes(), data.getWidth(), data.getHeight(), data.getWidth() * 4, 10);
Files.write(Path.of("output.webp"), newImageBytes);
```
