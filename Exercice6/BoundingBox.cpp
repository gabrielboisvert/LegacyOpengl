#include "pch.h"
#include "BoundingBox.h"
#include <GL\glew.h>

BoundingBox::BoundingBox()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;
    this->width = 1;
    this->height = 1;
    this->depth = 1;
}

float BoundingBox::getX()
{
    return this->x;
}

float BoundingBox::getY()
{
    return this->y;
}

float BoundingBox::getZ()
{
    return this->z;
}

float BoundingBox::getWidth()
{
    return this->width;
}

float BoundingBox::getHeight()
{
    return this->height;
}

float BoundingBox::getDepth()
{
    return this->depth;
}

void BoundingBox::setWidth(float width)
{
	this->width = width;
}

void BoundingBox::setHeight(float height)
{
	this->height = height;
}

void BoundingBox::setDepth(float depth)
{
	this->depth = depth;
}

void BoundingBox::applyTransform()
{
    float M[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, M);

    this->x = M[0] * -0.5f + M[4] * -0.5f + M[8] * 0.5f + M[12] * 1.f;
    this->y = M[1] * -0.5f + M[5] * -0.5f + M[9] * 0.5f + M[13] * 1.f;
    this->z = M[2] * -0.5f + M[6] * -0.5f + M[10] * 0.5f + M[14] * 1.f;
    this->w = M[3] * -0.5f + M[7] * -0.5f + M[11] * 0.5f + M[15] * 1.f;
}

const bool BoundingBox::operator==(const BoundingBox& box) const
{
    return &box == this;
}
