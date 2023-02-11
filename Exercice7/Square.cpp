#include "pch.h"
#include "Square.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

Square::Square()
{
	this->textureID = 0;
}

void Square::setTexture(unsigned int id)
{
	this->textureID = id;
}

void Square::draw()
{
	glBindTexture(GL_TEXTURE_2D, this->textureID);

	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0, 0, 1.0f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.5f, 0.5f, 0.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(-0.5f, 0.5f, 0.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.5f, -0.5f, 0.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(-0.5f, -0.5f, 0.f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

Square::~Square() {}