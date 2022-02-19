package me.tagavari.libwebp4j.data;

public class WebPBitstreamFeatures {
    private int width;
    private int height;
    private boolean hasAlpha;
    private boolean hasAnimation;
    private Format format;
    
    WebPBitstreamFeatures(int width, int height, boolean hasAlpha, boolean hasAnimation, Format format) {
        this.width = width;
        this.height = height;
        this.hasAlpha = hasAlpha;
        this.hasAnimation = hasAnimation;
        this.format = format;
    }
    
    public int getWidth() {
        return width;
    }
    
    public int getHeight() {
        return height;
    }
    
    public boolean isHasAlpha() {
        return hasAlpha;
    }
    
    public boolean isHasAnimation() {
        return hasAnimation;
    }
    
    public Format getFormat() {
        return format;
    }
    
    public enum Format {
        UNDEFINED,
        LOSSY,
        LOSSLESS
    }
}