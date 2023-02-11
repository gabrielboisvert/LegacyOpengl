#include "pch.h"
#include "Maze.h"
#include <time.h>
#include <iostream>

Cell::Cell()
{
	this->x = 0;
	this->y = 0;
	this->parent = nullptr;
}

Cell::Cell(unsigned int x, unsigned int y, CellPtr parent)
{
	this->x = x;
	this->y = y;
	this->parent = parent;
}

CellPtr Cell::getChildren()
{
	return CellPtr(new Cell((this->x * 2) - parent->x, (this->y * 2) - parent->y, nullptr));
}

const bool Cell::operator==(const Cell& cell) const
{
	return this->x == cell.x && this->y == cell.y;
}

const bool Cell::operator!=(const Cell& cell) const
{
	return !(*this == cell);
}

//--------------------------------------------------------

Maze::Maze()
{
	this->width = 0;
	this->height = 0;
}

void Maze::init(unsigned int width, unsigned int height)
{
	srand(unsigned int(time(NULL)));

	this->width = (width % 2) == 1 ? width : width - 1;
	this->height = (height % 2) == 1 ? height : height - 1;

	for (unsigned int i = 0; i < this->width; i++)
	{
		std::vector<char> tmpRow;
		for (unsigned int j = 0; j < this->height; j++)
			tmpRow.push_back('#');
		this->grid.push_back(tmpRow);
	}

	this->generate();
	this->graph = ai::Graph(this->grid);
	this->dj.pathFinder(this->graph);
	this->generateDoor();
}

bool Maze::isPathable(int x, int y)
{
	return (this->isValid(x, y)) && (this->grid[x][y] != '#') && (this->grid[x][y] != '.');
}

bool Maze::isValid(int x, int y)
{
	return x >= 0 && x < int(this->width) && y >= 0 && y < int(this->height);
}

bool Maze::isWall(int x, int y)
{
	return this->isValid(x, y) && this->grid[x][y] == '#';;
}

std::vector<CellPtr> Maze::getNeighbors(CellPtr cell, Cell_t type)
{
	std::vector<CellPtr> neighbors;

	int offSet[4][2]
	{
		{-1, 0}, //Up
		{1, 0}, //Down
		{0, 1}, //Right
		{-1, 0}, //Left
	};

	for (unsigned int i = 0; i < 4; i++)
	{
		int neighborX = cell->x + offSet[i][0];
		int neighborY = cell->y + offSet[i][1];

		bool validFloor = (type == Cell_t::FLOOR) && this->isPathable(neighborX, neighborY);
		bool validWall = (type == Cell_t::WALL) && (isWall(neighborX, neighborY));
		
		if (validFloor || validWall)
			neighbors.push_back(CellPtr(new Cell(neighborX, neighborY, cell)));
	}

	return neighbors;
}

void Maze::print()
{
	for (unsigned int i = 0; i < this->width; i++)
	{
		for (unsigned int j = 0; j < this->height; j++)
			std::cout << grid[i][j];
		std::cout << std::endl;
	}
}

void Maze::generate()
{
	//Place start point
	CellPtr start = CellPtr(new Cell(0, 0, nullptr));
	this->grid[0][0] = 'S';

	std::vector<CellPtr> frontier;
	frontier.push_back(CellPtr(new Cell(1, 0, start)));
	frontier.push_back(CellPtr(new Cell(0, 1, start)));

	while (!frontier.empty())
	{
		int randPoint = rand() % frontier.size();
		CellPtr cell = frontier[randPoint];
		frontier.erase(frontier.begin() + randPoint);

		CellPtr child = cell->getChildren();
		if (this->isValid(child->x, child->y) && this->grid[child->x][child->y] == '#')
		{
			this->grid[cell->x][cell->y] = ' ';
			this->grid[child->x][child->y] = 'E';

			std::vector<CellPtr> neighbors = this->getNeighbors(child, Cell_t::WALL);
			for (unsigned int i = 0; i < neighbors.size(); i++)
				frontier.push_back(neighbors[i]);

			this->grid[child->x][child->y] = ' ';
		}
	}

	//Place the end point
	this->grid[this->width - 1][this->height - 1] = 'E';

	return;
}

void Maze::generateDoor()
{
	ai::Node::EdgeList& path = this->dj.getPath();
	std::list<ai::Node>& nodes = this->graph.getNodes();
	int doorRange = int(path.size() / 4);


	//First door
	int min = 2;
	int max = doorRange;
	int idx = min + rand() % ((max + 1) - min);
	std::pair<unsigned int, unsigned int> xyPos = this->dj.getMapIdx(path[idx].getTo()->getIndex(), unsigned int(this->grid[0].size()));
	this->grid[xyPos.first][xyPos.second] = 'A';
	//key
	int cleIdx = (min - 1) + rand() % (idx - (min - 1));
	std::list<ai::Node>::iterator it = nodes.begin();
	std::advance(it, cleIdx);
	xyPos = this->dj.getMapIdx((*it).getIndex(), unsigned int(this->grid[0].size()));
	this->grid[xyPos.first][xyPos.second] = 'a';


	//Second door
	min = doorRange + 2;
	max = min + doorRange + 3;
	idx = min + rand() % ((max + 1) - min);
	xyPos = this->dj.getMapIdx(path[idx].getTo()->getIndex(), unsigned int(this->grid[0].size()));
	this->grid[xyPos.first][xyPos.second] = 'B';
	//key
	cleIdx = (min - 1) + rand() % (idx - (min - 1));
	it = nodes.begin();
	std::advance(it, cleIdx);
	xyPos = this->dj.getMapIdx((*it).getIndex(), unsigned int(this->grid[0].size()));
	this->grid[xyPos.first][xyPos.second] = 'b';


	//Third door
	min = (doorRange * 2) + 2;
	max = min + doorRange + 6;
	idx = min + rand() % ((max + 1) - min);
	xyPos = this->dj.getMapIdx(path[idx].getTo()->getIndex(), unsigned int(this->grid[0].size()));
	this->grid[xyPos.first][xyPos.second] = 'C';
	//key
	cleIdx = (min - 1) + rand() % (idx - (min - 1));
	it = nodes.begin();
	std::advance(it, cleIdx);
	xyPos = this->dj.getMapIdx((*it).getIndex(), unsigned int(this->grid[0].size()));
	this->grid[xyPos.first][xyPos.second] = 'c';
}
