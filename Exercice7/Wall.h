#pragma once
#include "Cube.h"
#include "BoundingBox.h"

class Wall : public BoundingBox
{
private:
	unsigned int textureID;
	Cube* cube;
public:
	Wall(Cube& cube, unsigned int textureID);

	void draw() override;

	~Wall();
};

