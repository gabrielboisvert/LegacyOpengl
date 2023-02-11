#include "pch.h"
#include "BMP.h"
#include <fstream>
#include <iostream>
#include <GL\glew.h>

unsigned int BMP::glFormat()
{
	return this->channel == 4 ? GL_BGRA : GL_BGR;
}

void BMP::loadFile(const char* fileName)
{

	std::ifstream file(fileName, std::ios::binary);
	if (!file.is_open())
	{
		std::cout << "Can't find file" << std::endl;
		return;
	}

	char header[HEADER_SIZE];
	file.read(&header[0], HEADER_SIZE);

	if (header[0] != 'B' && header[1] != 'M')
	{
		std::cout << "File not supported" << std::endl;
		return;
	}

	this->width = *(unsigned int*)&header[IMG_WIDTH_IDX];
	this->height = *(unsigned int*)&header[IMG_HEIGHT_IDX];
	this->channel = header[IMG_CHANNEL_IDX] / 8;
	if (this->channel != 4 && this->channel != 3)
	{
		std::cout << "Channel unsupported" << std::endl;
		return;
	}
	
	unsigned int imageSize = *(int*)&(header[IMG_SIZE_IDX]);
	if (imageSize == 0)
		imageSize = this->width * this->height * this->channel;
	this->datas = new char[(*(unsigned int*)&header[IMG_SIZE_IDX])];

	file.read(this->datas, imageSize);

	file.close();
}
