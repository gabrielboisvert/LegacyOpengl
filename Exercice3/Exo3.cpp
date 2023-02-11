#include "pch.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

enum Axis
{
	X,
	Y,
	Z,
	SIZE_AXIS
};

struct Point
{
	float x;
	float y;
	float z;
};

struct RotationComponant
{
	bool shouldRotate = false;
	float yaw = 0.0f;
};

bool showRepere;
unsigned char lastFormKey;
unsigned char lastColor;
unsigned char lastBackgroundColor;
bool showEdge;
RotationComponant rotationComponent[Axis::SIZE_AXIS];
float scale = 1.0f;

void drawEdre(unsigned int sideNumber)
{
	float angleIncrement = 360.0f / sideNumber;
	angleIncrement *= float(M_PI / 180.0f);
	float yaw = 0.0f;
	glBegin(GL_TRIANGLE_FAN);
	for (unsigned int i = 0; i < sideNumber; i++) {
		glVertex3f(0.5f * cosf(yaw), 0.5f * sinf(yaw), 0);
		yaw += angleIncrement;
	}
	glEnd();

	yaw = 0.0f;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 1);
	for (unsigned int i = 0; i < sideNumber; i++) {
		glVertex3f(0.5f * cosf(yaw), 0.5f * sinf(yaw), 0);
		yaw += angleIncrement;
	}
	glEnd();
}

void drawRepere(float width)
{
	//Line
	glColor3f(1, 0, 0);
	glPushMatrix();
		glScalef(scale, 1.0f, 1.0f);

		glPushMatrix();
			glTranslatef(width, 0, 0);
			glRotatef(90, 0, 1, 0);
			glScalef(0.2f, 0.2f, 0.2f);
			drawEdre(45);
		glPopMatrix();

		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(width, 0.0, 0.0);
		glEnd();
	glPopMatrix();

	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
		glScalef(1, scale, 1);

		glPushMatrix();
			glTranslatef(0, width, 0);
			glRotatef(-90, 1, 0, 0);
			glScalef(0.2f, 0.2f, 0.2f);
			drawEdre(45);
		glPopMatrix();

		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.0, width, 0.0);
		glEnd();
	glPopMatrix();

	glColor3f(0.0, 0.0, 1.0f);
	glPushMatrix();
		glScalef(1, 1, scale);
		
		glPushMatrix();
			glTranslatef(0, 0, width);
			glScalef(0.2f, 0.2f, 0.2f);
			drawEdre(45);
		glPopMatrix();
		
		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.0, 0.0, width);
		glEnd();
	glPopMatrix();
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

void drawShape()
{
	glPushMatrix();

	if (rotationComponent[Axis::X].shouldRotate)
		rotationComponent[Axis::X].yaw += 1.0f;
	if (rotationComponent[Axis::Y].shouldRotate)
		rotationComponent[Axis::Y].yaw += 1.0f;
	if (rotationComponent[Axis::Z].shouldRotate)
		rotationComponent[Axis::Z].yaw += 1.0f;

	glRotatef(rotationComponent[Axis::X].yaw, 1, 0, 0);
	glRotatef(rotationComponent[Axis::Y].yaw, 0, 1, 0);
	glRotatef(rotationComponent[Axis::Z].yaw, 0, 0, 1);

	glScalef(scale, scale, scale);

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
	glPopMatrix();
}

void ChangeBackgroundColor()
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

void Render(void)
{
	ChangeBackgroundColor();
	
	glClear(GL_COLOR_BUFFER_BIT);

	if (showRepere)
		drawRepere(2.0f);

	changeShapeColor();

	if (showEdge)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	drawShape();

	glutSwapBuffers();

	if (rotationComponent[Axis::X].shouldRotate || rotationComponent[Axis::Y].shouldRotate || rotationComponent[Axis::Z].shouldRotate)
		glutPostRedisplay();
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
	else if (key == 'x')
		rotationComponent[Axis::X].shouldRotate = !rotationComponent[Axis::X].shouldRotate;
	else if (key == 'y')
		rotationComponent[Axis::Y].shouldRotate = !rotationComponent[Axis::Y].shouldRotate;
	else if (key == 'z')
		rotationComponent[Axis::Z].shouldRotate = !rotationComponent[Axis::Z].shouldRotate;
	else if (key == 'e')
		showEdge = !showEdge;
	else if (key == '-')
		scale -= 0.1f;
	else if (key == '+')
		scale += 0.1f;
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
	gluLookAt(2, 2, 2, //Eye position 
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