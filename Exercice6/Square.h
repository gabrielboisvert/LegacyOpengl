#pragma once
class Square
{
private:
	unsigned int textureID;

public:
	Square();

	void setTexture(unsigned int id);

	void draw();

	~Square();
};