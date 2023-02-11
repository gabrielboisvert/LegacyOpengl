#pragma once
#include <memory>
#include <vector>
#include "PathFinding.h"

class Cell;

typedef std::shared_ptr<Cell> CellPtr;

class Cell
{
public:
	unsigned int x;
	unsigned int y;
	CellPtr parent;

	Cell();
	Cell(unsigned int x, unsigned int y, CellPtr parent);
	CellPtr getChildren();

	const bool operator==(const Cell& cell) const;
	const bool operator!=(const Cell& cell) const;
};

class Maze
{
private:
	enum class Cell_t
	{
		WALL,
		FLOOR
	};

	unsigned int width;
	unsigned int height;
	std::vector<std::vector<char>> grid;
	ai::Graph graph;
	ai::Dijkstra dj;

	void generate();
	void generateDoor();

public:
	Maze();

	void init(unsigned int width, unsigned int height);

	unsigned int getWidth() { return this->width; }
	unsigned int getHeight() { return this->height; }

	std::vector<std::vector<char>>& getGrid() 
	{ 
		return this->grid; 
	}

	bool isPathable(int x, int y);

	bool isValid(int x, int y);

	bool isWall(int x, int y);

	std::vector<CellPtr> getNeighbors(CellPtr cell, Cell_t type);

	void print();
};

