#include "pch.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

struct Point
{
	float x;
	float y;
	float z;
};

bool showRepere;
unsigned char lastFormKey;
unsigned char lastColor;
unsigned char lastBackgroundColor;
bool showEdge;

void drawRepere()
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(2.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 2.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 2.0);
	glEnd();
}

void drawCube()
{
	//Front
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glEnd();

	//Back
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();

	//Bottom
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glEnd();

	//Right
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glEnd();

	//Top
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glEnd();

	//Left
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glEnd();
}

void drawEdre(unsigned int sideNumber)
{
	float angleIncrement = 360.0f / sideNumber;
	angleIncrement *= float(M_PI / 180.0f);
	float yaw = 0.0f;
	glBegin(GL_TRIANGLE_FAN);
	for (unsigned int i = 0; i < sideNumber; i++) {
		glVertex3f(0.5f * cosf(yaw), 0.5f * sinf(yaw), 0.0f);
		yaw += angleIncrement;
	}
	glEnd();

	yaw = 0.0f;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 1);
	for (unsigned int i = 0; i < sideNumber; i++) {
		glVertex3f(0.5f * cosf(yaw), 0.5f * sinf(yaw), 0.0f);
		yaw += angleIncrement;
	}
	glEnd();
}

void drawSphere()
{
	float radius = 1.0f;
	unsigned int nbTriangle = 20;

	for (float angleZ = 0.0f; angleZ < M_PI; angleZ += float(M_PI / nbTriangle))
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (float angleXY = 0.0f; angleXY < 2.01f * M_PI; angleXY += float(M_PI / nbTriangle))
		{
			float x = radius * cosf(angleXY) * sinf(angleZ);
			float y = radius * sinf(angleXY) * sinf(angleZ);
			float z = radius * cosf(angleZ);
			glVertex3f(x, y, z);
			x = radius * cosf(angleXY) * sinf(angleZ + float(M_PI / nbTriangle));
			y = radius * sinf(angleXY) * sinf(angleZ + float(M_PI / nbTriangle));
			z = radius * cosf(angleZ + float(M_PI / nbTriangle));
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}

void changeBackgroundColor()
{
	switch (lastBackgroundColor)
	{
	case 'R': glClearColor(1, 0, 0, 1.0f);
		break;
	case 'G': glClearColor(0, 1, 0, 1.0f);
		break;
	case 'B': glClearColor(0, 0, 1, 1.0f);
		break;
	default: glClearColor(0, 0, 0, 1.0f);
		break;
	}
}

void changeShapeColor()
{
	switch (lastColor)
	{
	case 'r': glColor3f(1, 0, 0);
		break;
	case 'g': glColor3f(0, 1, 0);
		break;
	case 'b': glColor3f(0, 0, 1);
		break;
	default: glColor3f(1, 1, 1);
		break;
	}
}

void drawShape()
{
	switch (lastFormKey)
	{
	case '4': drawEdre(4);
		break;
	case '5': drawEdre(5);
		break;
	case '6': drawCube();
		break;
	case '7': drawEdre(7);
		break;
	case '8': drawEdre(8);
		break;
	case '9': drawEdre(9);
		break;
	case '0': drawSphere();
		break;
	default: break;
	}
}

void Render(void)
{
	changeBackgroundColor();
	
	glClear(GL_COLOR_BUFFER_BIT);

	if (showRepere)
		drawRepere();

	changeShapeColor();

	if (showEdge)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	drawShape();

	glutSwapBuffers();
}

void KeyboardInput(unsigned char key, int x, int y)
{
	if (key == '3')
		showRepere = !showRepere;
	else if ((key >= '4' && key <= '9') || key == '0')
		lastFormKey = key;
	else if (key == 'r' || key == 'g' || key == 'b')
		lastColor = key;
	else if (key == 'R' || key == 'G' || key == 'B')
		lastBackgroundColor = key;
	else if (key == '-')
		showEdge = !showEdge;
	else if (key == 'q' || key == 'Q')
		exit(0);

	glutPostRedisplay();
}

void setCameraPosition()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1280 / 700, 1, 40);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2, 3, 2, //Eye position 
		0, 0, 0, // origin, center
		0, 0, 1); // Axe to be top
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 700);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Hello Window");
	//glEnable(GL_CULL_FACE);

	setCameraPosition();

	glutDisplayFunc(Render);
	glutKeyboardFunc(KeyboardInput);
	glutMainLoop();
}