#pragma once
#include "ImageLoader.h"
class BMP : public ImageLoader
{
private:
	static const unsigned int HEADER_SIZE = 54;
	static const unsigned int IMG_WIDTH_IDX = 18;
	static const unsigned int IMG_HEIGHT_IDX = 22;
	static const unsigned int IMG_CHANNEL_IDX = 28;
	static const unsigned int IMG_SIZE_IDX = 34;

public:
	unsigned int glFormat() override;
	void loadFile(const char* fileName) override;
};

