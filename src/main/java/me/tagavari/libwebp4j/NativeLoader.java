package me.tagavari.libwebp4j;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

class NativeLoader {
	private static boolean isLoaded = false;

	private static final String nativeLibraryName = "libwebp4j";
	private static final String[] nativeLibraryExtensions = {".so", ".dll", ".dylib"};

	/**
	 * Loads native libraries
	 */
	static void load() {
		if(isLoaded) return;

		//Find a matching library
		URL resourceURL = null;
		String resourceExtension = null;
		for(String extension : nativeLibraryExtensions) {
			resourceURL = NativeLoader.class.getClassLoader().getResource(nativeLibraryName + extension);
			if(resourceURL != null) {
				resourceExtension = extension;
				break;
			}
		}

		if(resourceURL == null) {
			throw new RuntimeException("Failed to load native library " + nativeLibraryName);
		}

		//Copy the library to a temporary location
		Path extractedLibrary;
		try {
			Path extractedLibraryDir = Files.createTempDirectory(null);
			extractedLibrary = extractedLibraryDir.resolve(nativeLibraryName + resourceExtension);
			
			try(InputStream resourceStream = resourceURL.openStream()) {
				Files.copy(resourceStream, extractedLibrary, StandardCopyOption.REPLACE_EXISTING);
			}
		} catch(IOException exception) {
			throw new RuntimeException("Failed to copy temporary file for native library " + nativeLibraryName, exception);
		}

		//Load the library
		System.load(extractedLibrary.toString());

		isLoaded = true;
	}
}
