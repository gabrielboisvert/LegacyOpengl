#include "pch.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

struct Point
{
	float x;
	float y;
};

unsigned char lastFormKey;
unsigned char lastColor;
unsigned char lastBackgroundColor;
bool showEdge;
Point firstVertice;

void drawPoint()
{
	glPointSize(5);

	glBegin(GL_POINTS);
	glVertex2f(0.0f + firstVertice.x, 0.0f + firstVertice.y);
	glEnd();
}

void drawLine()
{
	glBegin(GL_LINES);
	glVertex2f(0.0f + firstVertice.x, 0.0f + firstVertice.y);
	glVertex2f(0.5f, 0.5f);
	glEnd();
}

void drawPolygone(unsigned int sideNumber)
{
	float angleIncrement = (360.0f / sideNumber) * float(M_PI / 180.0f);
	float yaw = 0.0f;
	glBegin(GL_TRIANGLE_FAN);
	for (unsigned int i = 0; i < sideNumber; i++) {
		if (i == 0)
			glVertex2f(0.5f * cosf(yaw) + firstVertice.x, 0.5f * sinf(yaw) + firstVertice.y);
		else
			glVertex2f(0.5f * cosf(yaw), 0.5f * sinf(yaw));
		yaw += angleIncrement;
	}
	glEnd();
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
	case '1': drawPoint();
		break;
	case '2': drawLine();
		break;
	case '3': drawPolygone(3);
		break;
	case '4': drawPolygone(4);
		break;
	case '5': drawPolygone(5);
		break;
	case '6': drawPolygone(6);
		break;
	case '7': drawPolygone(7);
		break;
	case '8': drawPolygone(8);
		break;
	case '9': drawPolygone(9);
		break;
	case '0': drawPolygone(360);
		break;
	default: break;
	}
}

void Render(void)
{
	changeBackgroundColor();
	
	glClear(GL_COLOR_BUFFER_BIT);

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
	if (key >= '0' && key <= '9')
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

void SpecialKeyboardInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_DOWN: firstVertice.y -= 0.1f;//down
		break;
	case GLUT_KEY_RIGHT: firstVertice.x += 0.1f;//right
		break;
	case GLUT_KEY_UP: firstVertice.y += 0.1f;//up
		break;
	case GLUT_KEY_LEFT: firstVertice.x -= 0.1f;//left
		break;
	default: break;
	}

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 700);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Hello Window");
	glEnable(GL_CULL_FACE);
	firstVertice = { 0, 0 };

	glutDisplayFunc(Render);
	glutKeyboardFunc(KeyboardInput);
	glutSpecialFunc(SpecialKeyboardInput);
	glutMainLoop();
}