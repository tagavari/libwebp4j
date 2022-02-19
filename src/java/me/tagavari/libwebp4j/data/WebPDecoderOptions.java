package me.tagavari.libwebp4j.data;

public class WebPDecoderOptions {
	private final boolean bypassFiltering;
	private final boolean noFancyUpscaling;
	private final boolean useCropping;
	private final int cropLeft, cropTop;
	private final int cropWidth, cropHeight;
	private final boolean useScaling;
	private final int scaledWidth, scaledHeight;
	private final boolean useThreads;
	private final int ditheringStrength;
	private final boolean flip;
	private final int alphaDitheringStrength;

	private WebPDecoderOptions(boolean bypassFiltering, boolean noFancyUpscaling, boolean useCropping, int cropLeft,
			int cropTop, int cropWidth, int cropHeight, boolean useScaling, int scaledWidth, int scaledHeight,
			boolean useThreads, int ditheringStrength, boolean flip, int alphaDitheringStrength) {
		this.bypassFiltering = bypassFiltering;
		this.noFancyUpscaling = noFancyUpscaling;
		this.useCropping = useCropping;
		this.cropLeft = cropLeft;
		this.cropTop = cropTop;
		this.cropWidth = cropWidth;
		this.cropHeight = cropHeight;
		this.useScaling = useScaling;
		this.scaledWidth = scaledWidth;
		this.scaledHeight = scaledHeight;
		this.useThreads = useThreads;
		this.ditheringStrength = ditheringStrength;
		this.flip = flip;
		this.alphaDitheringStrength = alphaDitheringStrength;
	}

	public boolean bypassFiltering() {
		return bypassFiltering;
	}

	public boolean noFancyUpscaling() {
		return noFancyUpscaling;
	}
	
	public boolean useCropping() {
		return useCropping;
	}
	
	public int getCropLeft() {
		return cropLeft;
	}
	
	public int getCropTop() {
		return cropTop;
	}
	
	public int getCropWidth() {
		return cropWidth;
	}

	public int getCropHeight() {
		return cropHeight;
	}
	
	public boolean useScaling() {
		return useScaling;
	}
	
	public int getScaledWidth() {
		return scaledWidth;
	}
	
	public int getScaledHeight() {
		return scaledHeight;
	}
	
	public boolean useThreads() {
		return useThreads;
	}
	
	public int getDitheringStrength() {
		return ditheringStrength;
	}
	
	public boolean doFlip() {
		return flip;
	}
	
	public int getAlphaDitheringStrength() {
		return alphaDitheringStrength;
	}
	
	public static class Builder {
		private boolean bypassFiltering = false;
		private boolean noFancyUpscaling = false;
		private boolean useCropping = false;
		private int cropLeft = 0, cropTop = 0;
		private int cropWidth = 0, cropHeight = 0;
		private boolean useScaling = false;
		private int scaledWidth = 0, scaledHeight = 0;
		private boolean useThreads = false;
		private int ditheringStrength = 0;
		private boolean flip = false;
		private int alphaDitheringStrength = 0;
		private WebPColorSpace colorSpace = WebPColorSpace.RGB;
		
		public Builder setBypassFiltering(boolean bypassFiltering) {
			this.bypassFiltering = bypassFiltering;
			return this;
		}
		public Builder setNoFancyUpscaling(boolean noFancyUpscaling) {
			this.noFancyUpscaling = noFancyUpscaling;
			return this;
		}
		public Builder setUseCropping(boolean useCropping) {
			this.useCropping = useCropping;
			return this;
		}
		public Builder setCropLeft(int cropLeft) {
			this.cropLeft = cropLeft;
			return this;
		}
		public Builder setCropTop(int cropTop) {
			this.cropTop = cropTop;
			return this;
		}
		public Builder setCropWidth(int cropWidth) {
			this.cropWidth = cropWidth;
			return this;
		}
		public Builder setCropHeight(int cropHeight) {
			this.cropHeight = cropHeight;
			return this;
		}
		public Builder setUseScaling(boolean useScaling) {
			this.useScaling = useScaling;
			return this;
		}
		public Builder setScaledWidth(int scaledWidth) {
			this.scaledWidth = scaledWidth;
			return this;
		}
		public Builder setScaledHeight(int scaledHeight) {
			this.scaledHeight = scaledHeight;
			return this;
		}
		public Builder setUseThreads(boolean useThreads) {
			this.useThreads = useThreads;
			return this;
		}
		public Builder setDitheringStrength(int ditheringStrength) {
			this.ditheringStrength = ditheringStrength;
			return this;
		}
		public Builder setFlip(boolean flip) {
			this.flip = flip;
			return this;
		}
		public Builder setAlphaDitheringStrength(int alphaDitheringStrength) {
			this.alphaDitheringStrength = alphaDitheringStrength;
			return this;
		}

		public WebPDecoderOptions build() {
			return new WebPDecoderOptions(
				bypassFiltering,
				noFancyUpscaling,
				useCropping,
				cropLeft, cropTop,
				cropWidth, cropHeight,
				useScaling,
				scaledWidth, scaledHeight,
				useThreads,
				ditheringStrength,
				flip,
				alphaDitheringStrength
			);
		}
	}
}
