package me.tagavari.libwebp4j;

import me.tagavari.libwebp4j.data.WebPColorSpace;
import me.tagavari.libwebp4j.data.WebPDecodedData;
import me.tagavari.libwebp4j.data.WebPDecoderOptions;
import me.tagavari.libwebp4j.exception.WebPException;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Objects;

import static org.junit.jupiter.api.Assertions.*;

class WebPTest {
	//Load resource files
	private final URL webpFile = Objects.requireNonNull(getClass().getClassLoader().getResource("test.webp"));
	private final URL bmpFile = Objects.requireNonNull(getClass().getClassLoader().getResource("test.bmp"));
	
	@Test
	@DisplayName("Versions should be reported correctly")
	void testVersion() {
		assertTrue(WebPDecode.getVersion() > 0);
		assertTrue(WebPEncode.getVersion() > 0);
	}
	
	/**
	 * Gets a raw byte array of RGB pixels from a {@link BufferedImage}
	 * @param image The image to process
	 * @return The array of RGB data
	 */
	private byte[] getRGBData(BufferedImage image) {
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
		
		return imageBytes;
	}
	
	@Test
	@DisplayName("Decoded WebP should match BMP")
	void testDecodeWebP() throws IOException, URISyntaxException, WebPException {
		//Read the file
		byte[] webpBytes = Files.readAllBytes(Paths.get(webpFile.toURI()));
		
		//Decode the WebP file
		WebPDecodedData webPData = WebPDecode.decode(webpBytes,
			new WebPDecoderOptions.Builder().build(),
			WebPColorSpace.RGB);
		
		//Compare it with the BMP file
		BufferedImage bmpImage = ImageIO.read(bmpFile);
		byte[] rawData = getRGBData(bmpImage);
		
		assertEquals(webPData.getWidth(), bmpImage.getWidth());
		assertEquals(webPData.getHeight(), bmpImage.getHeight());
		assertArrayEquals(webPData.getBytes(), rawData);
	}
	
	/* @Test
	@DisplayName("Encode -> Decode WebP data should match")
	void testEncodeWebP() throws IOException, WebPException {
		//Read the BMP file
		BufferedImage bmpImage = ImageIO.read(bmpFile);
		byte[] bmpData = getRGBData(bmpImage);
		
		//Encode the file to WebP
		byte[] webpBytes = WebPEncode.encodeLosslessRGB(bmpData, bmpImage.getWidth(), bmpImage.getHeight(), bmpImage.getWidth() * 3);
		
		//Decode the file back to raw bytes
		WebPDecodedData decodedData = WebPDecode.decode(webpBytes,
			new WebPDecoderOptions.Builder().build(),
			WebPColorSpace.RGB);
		
		assertEquals(decodedData.getWidth(), bmpImage.getWidth());
		assertEquals(decodedData.getHeight(), bmpImage.getHeight());
		assertEquals(decodedData.getBytes(), bmpData);
	} */
}