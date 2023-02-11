#include "pch.h"
#include "Cube.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

Cube::Cube() {}

void Cube::draw()
{
	//Front
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0, 1);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 1);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(1, 0);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glEnd();

	//Back
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(0, 1);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(1, 0);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 1);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();

	//Bottom
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(1, 1);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 0);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(0, 1);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glEnd();
	
	//Right
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(1, 0);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 1);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(0, 0);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(0, 1);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glEnd();
	
	//Top
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0, 1);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(1, 1);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(1, 0);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glEnd();

	//Left
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(0, 1);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(1, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 1);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glEnd();
}

Cube::~Cube() {}
