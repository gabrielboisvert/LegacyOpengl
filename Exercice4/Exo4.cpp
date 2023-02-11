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

struct Line
{
	std::vector<Point> points;
	bool finish = false;
	int pointIdx = 0;
	int nextPoint = 1;
	float ration = 0.0f;
};

bool showRepere;
unsigned char lastFormKey;
bool showEdge;
RotationComponant rotationComponent[Axis::SIZE_AXIS];
float scale = 1.0f;
Line lines;


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

void drawCylindre(unsigned int sideNumber, float radius, float height)
{
	//bottom, Top
	float angleIncrement = (360.0f / sideNumber) * float(M_PI / 180.0f);
	float yaw = 0.0f;
	glBegin(GL_TRIANGLE_FAN);
	for (unsigned int i = 0; i < sideNumber; i++) {
		glVertex3f(radius * cosf(yaw), radius * sinf(yaw), 0.0f);
		glVertex3f(radius * cosf(yaw), radius * sinf(yaw), height);
		yaw += angleIncrement;
	}
	glEnd();

	//Tube
	yaw = 0.0f;
	glBegin(GL_TRIANGLE_STRIP);
	for (unsigned int i = 0; i < sideNumber; i++) {
		glVertex3f(radius * cosf(yaw), radius * sinf(yaw), 0);
		glVertex3f(radius * cosf(yaw), radius * sinf(yaw), height);
		glVertex3f(radius * cosf(yaw + angleIncrement), radius * sinf(yaw + angleIncrement), 0);

		glVertex3f(radius * cosf(yaw + angleIncrement), radius * sinf(yaw + angleIncrement), height);
		yaw += angleIncrement;
	}
	glEnd();
}

void drawRepere(float width)
{
	//Z line
	glColor3f(0, 0, 1);
	glPushMatrix();
	glScalef(1, 1, scale);
	glPushMatrix();
		glTranslatef(0, 0, width);
		glScalef(0.05f, 0.05f, 0.05f);
		drawEdre(45);
	glPopMatrix();

	drawCylindre(45, 0.005f, width);
	
	glPopMatrix();

	//Y line
	glColor3f(0, 1, 0);
	glPushMatrix();
	glScalef(1, scale, 1);

	glPushMatrix();
		glTranslatef(0, width, 0);
		glRotatef(-90, 1, 0, 0);
		glScalef(0.05f, 0.05f, 0.05f);
		drawEdre(45);
	glPopMatrix();

	glRotatef(-90, 1, 0, 0);
	drawCylindre(45, 0.005f, width);
	glPopMatrix();
	
	//x line
	glColor3f(1, 0, 0);
	glPushMatrix();
	glScalef(scale, 1, 1);
	
	glPushMatrix();
		glTranslatef(width, 0, 0);
		glRotatef(90, 0, 1, 0);
		glScalef(0.05f, 0.05f, 0.05f);
		drawEdre(45);
	glPopMatrix();

	glRotatef(90, 0, 1, 0);
	drawCylindre(45, 0.005f, width);
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
	float radius = 0.25f;
	unsigned int nbTriangle = 20;

	for (float angleZ = 0.0f; angleZ < M_PI; angleZ += float( M_PI / nbTriangle))
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

void MoveObject()
{
	glTranslatef(lines.points[lines.pointIdx].x + (lines.ration * (lines.points[lines.nextPoint].x - lines.points[lines.pointIdx].x)),
				 lines.points[lines.pointIdx].y + (lines.ration * (lines.points[lines.nextPoint].y - lines.points[lines.pointIdx].y)),
				 lines.points[lines.pointIdx].z + (lines.ration * (lines.points[lines.nextPoint].z - lines.points[lines.pointIdx].z)));

	if (!rotationComponent[Axis::X].shouldRotate && !rotationComponent[Axis::Y].shouldRotate && !rotationComponent[Axis::Z].shouldRotate)
	{
		lines.ration += 0.005f;
		if (lines.ration >= 1.0f)
		{
			lines.pointIdx = (lines.pointIdx + int64_t(1)) % lines.points.size();
			lines.nextPoint = (lines.nextPoint + int64_t(1)) % lines.points.size();
			lines.ration = 0.0f;
		}
	}
}

void rotateObject()
{
	if (rotationComponent[Axis::X].shouldRotate)
		rotationComponent[Axis::X].yaw += 1.0f;
	if (rotationComponent[Axis::Y].shouldRotate)
		rotationComponent[Axis::Y].yaw += 1.0f;
	if (rotationComponent[Axis::Z].shouldRotate)
		rotationComponent[Axis::Z].yaw += 1.0f;

	glRotatef(rotationComponent[Axis::X].yaw, 1, 0, 0);
	glRotatef(rotationComponent[Axis::Y].yaw, 0, 1, 0);
	glRotatef(rotationComponent[Axis::Z].yaw, 0, 0, 1);
}

void drawShape()
{
	glPushMatrix();

	MoveObject();
	rotateObject();
	glScalef(scale, scale, scale);

	switch (lastFormKey)
	{
	case '1': drawSphere();
		break;
	case '2':	glScalef(0.4f, 0.4f, 0.4f);
				drawCube();
				break;
	default: break;
	}
	glPopMatrix();
}

void drawPoints()
{
	glPointSize(5);
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < lines.points.size(); i++)
		glVertex3f(lines.points[i].x, lines.points[i].y, lines.points[i].z);
	glEnd();
}

void drawLine()
{
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < lines.points.size(); i++)
		glVertex3f(lines.points[i].x, lines.points[i].y, lines.points[i].z);
	glEnd();
}

void Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (showEdge)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (showRepere)
	{
		drawRepere(0.9f);
		glColor3f(1.0f, 1.0f, 1.0f);
	}

	if (!lines.finish)
		drawPoints();
	else
	{
		drawLine();
		drawShape();
	}

	glutSwapBuffers();

	if (rotationComponent[Axis::X].shouldRotate || rotationComponent[Axis::Y].shouldRotate || rotationComponent[Axis::Z].shouldRotate || lines.finish)
		glutPostRedisplay();
}

void mouse(int key, int state, int x, int y)
{
	if (state == GLUT_UP)
		return;

	if (lines.finish)
		lines = Line();

	//Convert to ndc Coordinate
	float ndcX = ((float(x) / (glutGet(GLUT_WINDOW_WIDTH) / 2)) - 1.0f);
	float ndcY = -((float(y) / (glutGet(GLUT_WINDOW_HEIGHT) / 2)) - 1.0f);

	switch (key)
	{
	case GLUT_LEFT_BUTTON:	lines.points.push_back({ ndcX, ndcY, 0.0f });
		break;
	case GLUT_RIGHT_BUTTON: lines.points.push_back({ ndcX, ndcY, 0.0f });
							if (lines.points.size() > 1)
								lines.finish = true;
		break;
	default: break;
	}

	glutPostRedisplay();
}

void KeyboardInput(unsigned char key, int x, int y)
{
	if (key == '3')
		showRepere = !showRepere;
	else if (key == '1' || key == '2')
		lastFormKey = key;
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

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 700);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Hello Window");
	glEnable(GL_CULL_FACE);

	glutDisplayFunc(Render);
	glutKeyboardFunc(KeyboardInput);
	glutMouseFunc(mouse);

	glutMainLoop();
}