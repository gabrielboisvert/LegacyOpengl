#pragma once
#include "Cube.h"
#include "BoundingBox.h"

class Key : public BoundingBox
{
private:
	unsigned int textureID;
	Cube* cube;
	float angle;
	float rotationSpeed;
	char tag;
	int x;
	int y;
public:
	Key(Cube& cube, unsigned int textureID, char tag, int x, int y);
	int getX();
	int getY();
	char getTag();
	void rotate(float deltaTime);
	void draw() override;
	const bool operator==(const Key& key) const;
	~Key();
};

