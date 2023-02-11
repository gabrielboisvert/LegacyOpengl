#include "pch.h"
#include "Door.h"
#include <GL\glew.h>

Door::Door(Cube& cube, unsigned int textureID, char tag, int x, int y)
{
    this->cube = &cube;
    this->textureID = textureID;
    this->tag = tag;
    this->x = x;
    this->y = y;
}

int Door::getX()
{
    return this->x;
}

int Door::getY()
{
    return this->y;
}

char Door::getTag()
{
    return this->tag;
}

void Door::draw()
{
    if (this->cube == nullptr)
        return;

    glBindTexture(GL_TEXTURE_2D, this->textureID);
    this->cube->draw();
    glBindTexture(GL_TEXTURE_2D, 0);
}

Door::~Door() {}
