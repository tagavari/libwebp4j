package me.tagavari.libwebp4j.data;

public class WebPInfo {
    private int width;
    private int height;

    WebPInfo(int width, int height) {
        this.width = width;
        this.height = height;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }
}