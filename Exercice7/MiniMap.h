#pragma once
#include <vector>
#include "Square.h"


class MiniMap
{
private:
	unsigned int x;
	unsigned int y;

	bool showKey;

	void drawBackground();

	void drawSquare(float r, float g, float b);

public:
	MiniMap();

	void switchShowKey();

	void move(unsigned int x, unsigned int y);

	void draw(std::vector<std::vector<char>>& grid, Square& floors, Square& wall);
};

