#pragma once
#include "Cube.h"
#include "BoundingBox.h"

class Door : public BoundingBox
{
private:
	unsigned int textureID;
	Cube* cube;
	char tag;
	int x;
	int y;
public:
	Door(Cube& cube, unsigned int textureID, char tag, int x, int y);
	int getX();
	int getY();
	char getTag();
	void draw() override;
	~Door();
};

