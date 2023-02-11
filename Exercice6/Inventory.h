#pragma once
#include <vector>
#include "Key.h"

class Inventory
{
private:
	std::vector<Key*> keyObtained;

	void drawSquare(float r, float g, float b);

public:
	
	void addKey(Key* key);

	bool countainTag(char tag);

	void draw(unsigned int keyTexture);
};

