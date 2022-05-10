package me.tagavari.libwebp4j.exception;

import java.lang.Exception;

public class WebPException extends Exception {
	WebPException() {
	}
	
	WebPException(String message) {
		super(message);
	}
	
	WebPException(String message, Throwable cause) {
		super(message, cause);
	}
	
	WebPException(Throwable cause) {
		super(cause);
	}
}