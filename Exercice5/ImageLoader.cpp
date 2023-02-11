#include "pch.h"
#include "ImageLoader.h"
#include <GL\glew.h>

ImageLoader::ImageLoader()
{
	this->width = 0;
	this->height = 0;
	this->channel = 0;
	this->datas = nullptr;
}

unsigned int ImageLoader::getWidth()
{
	return this->width;
}

unsigned int ImageLoader::getHeight()
{
	return this->height;
}

unsigned int ImageLoader::getChannel()
{
	return this->channel;
}

unsigned int ImageLoader::glChannel()
{
	return this->channel == 4 ? GL_RGBA8 : GL_RGB;
}

char* ImageLoader::getDatas()
{
	return this->datas;
}

ImageLoader::~ImageLoader()
{
	delete[] this->datas;
}
