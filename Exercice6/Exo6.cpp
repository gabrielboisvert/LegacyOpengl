#include "pch.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Camera.h"
#include "Maze.h"
#include "Wall.h"
#include "Square.h"
#include "Door.h"
#include "Key.h"

Camera& cam = Camera::getInstance();
Cube cube;
std::vector<BoundingBox*> solidObject;
unsigned int wallTexture = 0;
std::vector<Wall> walls;
unsigned int doorTexture = 0;
unsigned int keyTexture = 0;
Square frontier;
Square floors;
std::vector<Door*> doors;
std::vector<Key*> keys;
Maze maze;

unsigned int CreateTextureFromBMP(const char* filename)
{
	// Each BMP file begins by a 54-bytes header
	unsigned char header[54];

	FILE* file;
	fopen_s(&file, filename, "rb");
	if (file == 0)
		return 0;

	fread(header, 1, 54, file);

	unsigned int imageSize = *(int*)&(header[34]);
	int width = *(int*)&(header[18]);
	int height = *(int*)&(header[22]);

	if (imageSize == 0)
		imageSize = width * height * 4;

	unsigned char* data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	if (file != 0)
		fclose(file);

	//Create texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_BGRA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] data;
	return texture;
}

void drawFloor()
{
	for (unsigned int i = 0; i < maze.getWidth(); i++)
		for (unsigned int j = 0; j < maze.getHeight(); j++)
		{
			if (i == 0 && j == 0)
				glColor3f(0, 0, 1);
			else if (i == maze.getWidth() - 1 && j == maze.getHeight() - 1)
				glColor3f(0, 1, 0);
			else
				glColor3f(1, 1, 1);

			glPushMatrix();
			glTranslatef(float(i), -1.f, -float(j));
			glRotatef(-90, 1, 0, 0);
			floors.draw();
			glPopMatrix();
		}

	glColor3f(1, 1, 1);
}

void drawWall()
{
	unsigned int k = 0;
	std::vector<std::vector<char>>& grid = maze.getGrid();
	for (unsigned int i = 0; i < maze.getWidth(); i++)
		for (unsigned int j = 0; j < maze.getHeight(); j++)
		{
			if (grid[i][j] != '#')
				continue;

			glPushMatrix();
			glTranslatef(float(i), 0.f, -float(j));
			glScalef(1.f, 2.f, 1.f);
			walls[k++].draw();
			glPopMatrix();
		}
}

void drawFrontier()
{
	std::vector<std::vector<char>>& grid = maze.getGrid();
	// Front Right Frontier
	for (unsigned int i = 0; i < maze.getWidth(); i++)
	{
		if (grid[i][0] == '#' || grid[i][0] == 'E')
			continue;

		glPushMatrix();
		glTranslatef(float(i), 0.f, 0.5f);
		glRotatef(180.f, 0, 1, 0);
		glScalef(1.f, 2.f, 1.f);
		frontier.draw();
		glPopMatrix();
	}

	// Front Left Frontier
	for (unsigned int i = 0; i < maze.getHeight(); i++)
	{
		if (grid[0][i] == '#' || grid[0][i] == 'E')
			continue;

		glPushMatrix();
		glTranslatef(-0.5f, 0.f, -float(i));
		glRotatef(90, 0, 1, 0);
		glScalef(1.f, 2.f, 1.f);
		frontier.draw();
		glPopMatrix();
	}
	
	// Back Left
	int y = maze.getHeight() - 1;
	for (unsigned int i = 0; i < maze.getWidth(); i++)
	{
		if (grid[i][y] == '#' || grid[i][y] == 'E')
			continue;

		glPushMatrix();
		glTranslatef(float(i), 0.f, -float(y) - 0.5f);
		glScalef(1.f, 2.f, 1.f);
		frontier.draw();
		glPopMatrix();
	}

	// Back Right
	int x = maze.getWidth() - 1;
	for (unsigned int i = 0; i < maze.getHeight(); i++)
	{
		if (grid[x][i] == '#')
			continue;

		glPushMatrix();
		glTranslatef(float(x) + 0.5f, 0.f, -float(i));
		glRotatef(-90, 0, 1, 0);
		glScalef(1.f, 2.f, 1.f);
		frontier.draw();

		glPopMatrix();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void drawDoor()
{
	unsigned int k = 0;
	std::vector<std::vector<char>>& grid = maze.getGrid();
	for (unsigned int i = 0; i < maze.getWidth(); i++)
		for (unsigned int j = 0; j < maze.getHeight(); j++)
		{
			char tag = grid[i][j];
			if (tag != 'A' && tag != 'B' && tag != 'C')
				continue;

			glPushMatrix();
			glTranslatef(float(i), 0.f, -float(j));
			glScalef(1.f, 2.f, 1.f);
			if (tag == 'A')
				glColor3f(1, 1, 0);
			else if (tag == 'B')
				glColor3f(1, 0, 1);
			else if (tag == 'C')
				glColor3f(1, 0.5f, 0);
			doors[k++]->draw();
			glColor3f(1, 1, 1);
			glPopMatrix();
		}
}

void drawKey()
{
	unsigned int k = 0;
	std::vector<std::vector<char>>& grid = maze.getGrid();
	for (unsigned int i = 0; i < maze.getWidth(); i++)
		for (unsigned int j = 0; j < maze.getHeight(); j++)
		{
			char tag = grid[i][j];
			if (tag != 'a' && tag != 'b' && tag != 'c')
				continue;

			glPushMatrix();
			glTranslatef(float(i), -0.25, -float(j));
			glScalef(0.25f, 0.5f, 0.25f);
			if (tag == 'a')
				glColor3f(1, 1, 0);
			else if (tag == 'b')
				glColor3f(1, 0, 1);
			else if (tag == 'c')
				glColor3f(1, 0.5f, 0);
			keys[k]->rotate(cam.getDeltaTime());
			keys[k++]->draw();
			glColor3f(1, 1, 1);
			glPopMatrix();
		}
}

void Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam.setPerpective();
	cam.update(solidObject, keys, maze, doors);

	drawFloor();
	drawWall();
	drawDoor();
	drawKey();
	drawFrontier();

	cam.setOrthogonal();
	cam.getMap().draw(maze.getGrid(), floors, frontier);

	cam.setInventoryCam();
	cam.getInventory().draw(keyTexture);

	glutSwapBuffers();
}

void createWindow(int& argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 700);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Hello Window");
}

void settingGlMode()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW); //default of opengl
}

void settingGlutCallBack()
{
	glutDisplayFunc(Render);
	glutIdleFunc(Render);

	glutMainLoop();
}

void settingMaze()
{
	maze.init(20, 25);
	//maze.print(); //To debug

	std::vector<std::vector<char>>& grid = maze.getGrid();
	for (unsigned int i = 0; i < maze.getWidth(); i++)
		for (unsigned int j = 0; j < maze.getHeight(); j++)
		{
			if (grid[i][j] == '#')
			{

				Wall* wall = new Wall(cube, wallTexture);

				glPushMatrix();
				glTranslatef(float(i), 0.f, -float(j));
				glScalef(1.f, 2.f, 1.f);
				wall->setHeight(2.f);
				wall->applyTransform();
				glPopMatrix();

				walls.push_back(std::move(*wall));
				solidObject.push_back(wall);
			}
			else if (grid[i][j] == 'A' || grid[i][j] == 'B' || grid[i][j] == 'C')
			{
				Door* door = new Door(cube, doorTexture, grid[i][j], i, j);

				glPushMatrix();
				glTranslatef(float(i), 0.f, -float(j));
				glScalef(1.f, 2.f, 1.f);
				door->setHeight(2.f);
				door->applyTransform();
				glPopMatrix();

				doors.push_back(door);
				solidObject.push_back(door);
			}
			else if (grid[i][j] == 'a' || grid[i][j] == 'b' || grid[i][j] == 'c')
			{
				Key* key = new Key(cube, keyTexture, grid[i][j], i, j);

				glPushMatrix();
				glTranslatef(float(i), -0.25, -float(j));
				glScalef(0.25f, 0.5f, 0.25f);
				key->setWidth(0.5f);
				key->setHeight(0.5f);
				key->setDepth(0.5f);
				key->applyTransform();
				glPopMatrix();

				keys.push_back(key);
				solidObject.push_back(key);
			}
		}
}

void settingCamera()
{
	cam.init(0, 0, float(maze.getWidth()), float(maze.getHeight()));
}

void settingTexture()
{
	floors.setTexture(CreateTextureFromBMP("floor.bmp"));
	wallTexture = CreateTextureFromBMP("wall.bmp");
	doorTexture = CreateTextureFromBMP("door.bmp");
	keyTexture = CreateTextureFromBMP("key.bmp");
	frontier.setTexture(wallTexture);
}

int main(int argc, char* argv[])
{
	createWindow(argc, argv);
	settingGlMode();
	settingTexture();
	settingMaze();
	settingCamera();
	settingGlutCallBack();
}