#include "pch.h"
#include "Key.h"
#include <GL\glew.h>

Key::Key(Cube& cube, unsigned int textureID, char tag, int x, int y)
{
    this->cube = &cube;
    this->textureID = textureID;
    this->rotationSpeed = 20.f;
    this->tag = tag;
    this->x = x;
    this->angle = 0.f;
    this->y = y;
}

int Key::getX()
{
    return this->x;
}

int Key::getY()
{
    return this->y;
}

char Key::getTag()
{
    return this->tag;
}

void Key::rotate(float deltaTime)
{
    this->angle += this->rotationSpeed * deltaTime;
    if (this->angle >= 360)
        this->angle -= 360;
    
    glRotatef(-this->angle, 0, 1, 0);
}

void Key::draw()
{
    if (this->cube == nullptr)
        return;

    glBindTexture(GL_TEXTURE_2D, this->textureID);
    this->cube->draw();
    glBindTexture(GL_TEXTURE_2D, 0);
}

const bool Key::operator==(const Key& key) const
{
    return this == &key;
}

Key::~Key() {}