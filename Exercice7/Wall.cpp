#include "pch.h"
#include "Wall.h"
#include <GL/glew.h>
#include <GL/freeglut.h>


Wall::Wall(Cube& cube, unsigned int textureID)
{
    this->cube = &cube;
    this->textureID = textureID;
}

void Wall::draw()
{
    if (this->cube == nullptr)
        return;

    glBindTexture(GL_TEXTURE_2D, this->textureID);
    this->cube->draw();
    glBindTexture(GL_TEXTURE_2D, 0);
}

Wall::~Wall() {}
