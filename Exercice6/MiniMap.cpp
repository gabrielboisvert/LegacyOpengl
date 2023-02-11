#include "pch.h"
#include "MiniMap.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

MiniMap::MiniMap()
{
	this->x = 0;
	this->y = 0;
	this->showKey = false;
}

void MiniMap::switchShowKey()
{
	this->showKey = !this->showKey;
}

void MiniMap::drawBackground()
{
	glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, 1);
	glVertex3f(1, -1, 1);
	glVertex3f(-1, -1, 1);
	glEnd();
	glColor3f(1, 1, 1);
}

void MiniMap::move(unsigned int x, unsigned int y)
{
	this->x = x;
	this->y = y;
}

void MiniMap::drawSquare(float r, float g, float b)
{
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0.5, 0.5, 0);
	glVertex3f(-0.5, 0.5, 0);
	glVertex3f(0.5, -0.5, 0);
	glVertex3f(-0.5, -0.5, 0);
	glEnd();
	glColor3f(1, 1, 1);
}

void MiniMap::draw(std::vector<std::vector<char>>& grid, Square& floors, Square& wall)
{
	this->drawBackground();

	float scaleX = float(2.f / (grid.size()));
	float scaleY = float(2.f / (grid[0].size()));

	for (unsigned int i = 0; i < grid.size(); i++)
		for (unsigned int j = 0; j < grid[i].size(); j++)
		{
			glPushMatrix();
			glTranslatef(-0.96f, -0.96f, 0.f);
			glTranslatef(i * scaleX, (j * scaleY), 0);
			glScalef(scaleX, scaleY, 1);
			
			if (this->x == i && this->y == j)
				this->drawSquare(1, 0, 0);
			else if (grid[i][j] == '#')
				wall.draw();
			else if (grid[i][j] == 'E')
				this->drawSquare(0, 1, 0);
			else if (grid[i][j] == 'S')
				this->drawSquare(0, 0, 1);
			else if (grid[i][j] == 'A' || (grid[i][j] == 'a' && this->showKey))
				this->drawSquare(1, 1, 0);
			else if (grid[i][j] == 'B' || (grid[i][j] == 'b' && this->showKey))
				this->drawSquare(1, 0, 1);
			else if (grid[i][j] == 'C' || (grid[i][j] == 'c' && this->showKey))
				this->drawSquare(1, 0.5, 0);
			else
				floors.draw();

			glPopMatrix();

			
		}
}
