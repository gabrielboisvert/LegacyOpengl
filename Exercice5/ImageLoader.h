#pragma once
class ImageLoader
{
protected:
	unsigned int width;
	unsigned int height;
	unsigned int channel;
	char* datas;

public:
	
	ImageLoader();

	unsigned int getWidth();
	unsigned int getHeight();
	unsigned int getChannel();
	unsigned int glChannel();
	virtual unsigned int glFormat() = 0;
	char* getDatas();

	virtual void loadFile(const char* fileName) = 0;

	~ImageLoader();
};

