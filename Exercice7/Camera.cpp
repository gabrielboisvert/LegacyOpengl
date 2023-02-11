#include "pch.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include "Camera.h"
#include <algorithm>
#include <iostream>

#define TO_RADIANS M_PI / 180.0f

Camera Camera::cam;

Camera::Camera()
{
	this->vPosition = { 0, 0, 0 };
	this->vDirection = { 0, 0, -1 };

	this->yaw = 0.f;
	this->pitch = 0.f;

	this->speed = 2.5f;
	this->sensitivity = 20.f;

	this->directionX = 0;
	this->directionZ = 0;

	this->minX = this->minZ = this->maxX = this->maxZ = 0;
	this->xCollide = this->zCollide = this->xSlideAxis = false;

	this->crouch = false;
	this->crouchPosition = 0.f;
	this->crouchSpeed = 2.f;
	this->capCrouch = 0.5f;

	this->sprint = false;
	this->showEdge = false;
	this->usedLight = -1;
}

Camera& Camera::getInstance()
{
	return Camera::cam;
}

void Camera::init(float minX, float minZ, float maxX, float maxZ)
{
	glutReshapeFunc(Camera::resize);
	glutSpecialFunc(Camera::spInput);
	glutSpecialUpFunc(Camera::spInputUP);
	glutKeyboardFunc(Camera::input);
	glutKeyboardUpFunc(Camera::inputUP);
	glutPassiveMotionFunc(Camera::mouse);

	//Cursor
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

	this->minX = minX;
	this->minZ = minZ;
	this->maxX = maxX;
	this->maxZ = maxZ;
	this->deltaTime.oldTime = glutGet(GLUT_ELAPSED_TIME);
}

float Camera::getDeltaTime()
{
	return this->deltaTime.dt;
}

void Camera::updateDT()
{
	int time = glutGet(GLUT_ELAPSED_TIME);
	cam.deltaTime.dt = (time - cam.deltaTime.oldTime) / 1000.0f;
	cam.deltaTime.oldTime = time;
}

void Camera::update(std::vector<BoundingBox*>& solidObject, std::vector<Key*> keys, Maze& maze, std::vector<Door*>& doors)
{
	this->updateDT();

	glLoadIdentity();
	this->move(solidObject, keys, maze, doors);

	if (this->usedLight != -1)
		glEnable(GL_LIGHTING);

	this->setLightPos();

	gluLookAt(vPosition.x, vPosition.y, vPosition.z,
		double(vPosition.x) + double(vDirection.x), double(vPosition.y) + double(vDirection.y), double(vPosition.z) + double(vDirection.z),
		0, 1, 0);
	
	this->crouchMove();
	if (usedLight == 0)
	{
		GLfloat lightPos[] = { float(maze.getGrid().size() / 2.f), 5.0f, -float(maze.getGrid()[0].size() / 2.f), 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	}
}

void Camera::activateLight()
{
	switch (this->usedLight)
	{
	case 0:
		glEnable(GL_LIGHT0);
		break;
	case 1:
		glEnable(GL_LIGHT1);
		break;
	case 2:
		glEnable(GL_LIGHT2);
		break;
	default: glDisable(GL_LIGHTING);
		break;
	}
}

void Camera::setLightPos()
{
	if (usedLight == 1)
	{
		GLfloat lightPos[] = { 0, 0, 0, 1.f };
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	}
	else if (usedLight == 2)
	{
		GLfloat lightPos[] = { 0, 0, 10.f, 1.f };
		glLightfv(GL_LIGHT2, GL_POSITION, lightPos);
	}
}

void Camera::disableLight()
{
	switch (this->usedLight)
	{
	case 0:
		glDisable(GL_LIGHT0);
		break;
	case 1:
		glDisable(GL_LIGHT1);
		break;
	case 2:
		glDisable(GL_LIGHT2);
		break;
	default:
		break;
	}
}

void Camera::crouchMove()
{
	if (this->crouch)
	{
		if (this->crouchPosition < this->capCrouch)
			this->crouchPosition += this->crouchSpeed * this->deltaTime.dt;
		else
			this->crouchPosition = this->capCrouch;
	}
	else
	{
		if (this->crouchPosition > 0)
			this->crouchPosition -= this->crouchSpeed * this->deltaTime.dt;
		else
			this->crouchPosition = 0;
	}
	glTranslatef(0, this->crouchPosition, 0);
}

void Camera::move(std::vector<BoundingBox*>& solidObject, std::vector<Key*> keys, Maze& maze, std::vector<Door*>& doors)
{
	float newPosX = this->vPosition.x;
	float newPosZ = this->vPosition.z;

	float speed = this->speed;
	if (this->sprint)
		speed *= 1.5f;
	else if (this->crouch)
		speed /= 3.f;

	Vec3 rightVec = crossProduct({ 0, 1, 0 }, this->vDirection);
	if (this->directionZ == 1) {
		newPosX += (vDirection.x * speed * this->deltaTime.dt);
		newPosZ += (vDirection.z * speed * this->deltaTime.dt);
	}
	else if (this->directionZ == -1) {
		newPosX -= (vDirection.x * speed * this->deltaTime.dt);
		newPosZ -= (vDirection.z * speed * this->deltaTime.dt);
	}
	if (this->directionX == -1) {
		newPosX += (rightVec.x * speed * this->deltaTime.dt);
		newPosZ += (rightVec.z * speed * this->deltaTime.dt);
	}
	else if (this->directionX == 1) {
		newPosX -= (rightVec.x * speed * this->deltaTime.dt);
		newPosZ -= (rightVec.z * speed * this->deltaTime.dt);
	}

	bool collide = this->isCollide(newPosX, newPosZ, solidObject, keys, maze, doors);

	if (!this->xCollide)
		this->vPosition.x = newPosX;
	if (!this->zCollide)
		this->vPosition.z = newPosZ;

	if (!collide)
		this->xCollide = this->zCollide = this->xSlideAxis = false;

	this->updateMapCoord();
}

template<typename Base, typename T>
inline bool instanceof(const T*) {
	return std::is_base_of<Base, T>::value;
}

bool Camera::isCollide(float& x, float& z, std::vector<BoundingBox*>& solidObject, std::vector<Key*> keys, Maze& maze, std::vector<Door*>& doors)
{
	//Check for fontiere collision
	if (x <= this->minX - 0.35f || x >= this->maxX - 0.65f)
		xCollide = true;

	if (z >= this->minZ + 0.35f || z <= -(this->maxZ - 0.65f))
		zCollide = true;

	//Check for wall Collision
	bool collide = false;
	for (unsigned int i = 0; i < solidObject.size(); i++)
	{
		BoundingBox* obj = solidObject[i];
		if (x >= obj->getX() - 0.15f && x <= obj->getX() + obj->getWidth() + 0.15f
			&& z <= obj->getZ() + 0.15f && z >= obj->getZ() - (obj->getDepth() + 0.15f))
		{
			Key* key = dynamic_cast<Key*>(obj);
			Door* door = dynamic_cast<Door*>(obj);
			if (key != nullptr)
			{
				solidObject.erase(std::find(solidObject.begin(), solidObject.end(), obj));
				keys.erase(std::find(keys.begin(), keys.end(), key));
				maze.getGrid()[key->getX()][key->getY()] = ' ';
				this->inventory.addKey(key);
				return false;
			}
			else if (door != nullptr)
			{
				if (this->inventory.countainTag(door->getTag() + 32))
				{
					solidObject.erase(std::find(solidObject.begin(), solidObject.end(), obj));
					doors.erase(std::find(doors.begin(), doors.end(), door));
					maze.getGrid()[door->getX()][door->getY()] = ' ';
					return false;
				}
			}

			this->checkFaceColliding(obj, x, z);
			collide = true;
		}
	}
	return collide;
}

void Camera::checkFaceColliding(BoundingBox* obj, float& x, float& z)
{
	std::pair<float, float> center = { obj->getX() + (obj->getWidth() / 2.f),  obj->getZ() - (obj->getDepth() / 2.f) };

	float distFrontSide = ((x - center.first) * (x - center.first)) + ((z - (center.second + (obj->getDepth() / 2.f))) * (z - (center.second + (obj->getDepth() / 2.f))));
	float distBackSide = ((x - center.first) * (x - center.first)) + ((z - (center.second - (obj->getDepth() / 2.f))) * (z - (center.second - (obj->getDepth() / 2.f))));

	float distLeftSide = ((x - (center.first - (obj->getWidth() / 2.f))) * (x - (center.first - (obj->getWidth() / 2.f)))) + ((z - center.second) * (z - center.second));
	float distRightSide = ((x - (center.first + (obj->getWidth() / 2.f))) * (x - (center.first + (obj->getWidth() / 2.f)))) + ((z - center.second) * (z - center.second));

	float min = std::min(std::min(distFrontSide, distBackSide), std::min(distLeftSide, distRightSide));

	if (min == distFrontSide || min == distBackSide)
		if (z <= obj->getZ() + 0.15f && z >= obj->getZ() - (obj->getDepth() + 0.15f))
			if (!this->xCollide)
			{
				this->zCollide = true;
				this->xSlideAxis = true;
			}

	if (min == distLeftSide || min == distRightSide)
		if (x >= obj->getX() - 0.15f && x <= obj->getX() + obj->getWidth() + 0.15f)
			if (!this->zCollide)
				this->xCollide = true;


	if (x >= obj->getX() && x <= obj->getX() + obj->getWidth() && !this->xSlideAxis)
		this->zCollide = true;
	else if (z <= obj->getZ() && z >= obj->getZ() - obj->getDepth() && this->xSlideAxis)
		this->xCollide = true;
}

void Camera::updateMapCoord()
{
	int mapPosX = int(abs(this->vPosition.x + 0.5f));
	int mapPosY = int(abs(this->vPosition.z - 0.5f));
	this->miniMap.move(mapPosX, mapPosY);
}

MiniMap& Camera::getMap()
{
	return this->miniMap;
}

Inventory& Camera::getInventory()
{
	return this->inventory;
}

void Camera::mouse(int x, int y)
{
	cam.inputModifiers();

	int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	int deltaX = centerX - x;
	int deltaY = centerY - y;

	//XZ rotation
	cam.yaw -= deltaX / cam.sensitivity;
	float angleXZ = cam.yaw * float(TO_RADIANS);
	cam.vDirection.x = sin(angleXZ);
	cam.vDirection.z = -cos(angleXZ);

	//Y rotation
	cam.pitch += deltaY / cam.sensitivity;
	if (cam.pitch >= 70)
		cam.pitch = 70;
	else if (cam.pitch <= -60)
		cam.pitch = -60;
	float angleY = cam.pitch * float(TO_RADIANS);
	cam.vDirection.y = sin(angleY);
	
	glutWarpPointer(centerX, centerY);
}

void Camera::inputModifiers()
{
	int state = glutGetModifiers();
	cam.crouch = (state == GLUT_ACTIVE_SHIFT);
	cam.sprint = (state == GLUT_ACTIVE_CTRL);
}

void Camera::input(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	case 23:
	case 'W':
		cam.directionZ = 1;
		break;
	case 's':
	case 19:
	case 'S':
		cam.directionZ = -1;
		break;
	case 'a':
	case 1:
	case 'A':
		cam.directionX = -1;
		break;
	case 'd':
	case 4:
	case 'D':
		cam.directionX = 1;
		break;
	case 'h':
	case 'H':
		cam.miniMap.switchShowKey();
		break;
	case 'e':
	case 'E':
		cam.showEdge = !cam.showEdge;
		if (cam.showEdge)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

	case '0':
	case '1':
	case '2':
		cam.disableLight();
		cam.usedLight = key - '0';
		cam.activateLight();
		break;

	case 'q':
	case 'Q':
		exit(0);
	default:
		break;
	}
}

void Camera::inputUP(unsigned char key, int x, int y)
{
	if (((key == 'w' || key == 'W' || key == 23) && cam.directionZ == 1) || ((key == 's' || key == 'S' || key == 19) && cam.directionZ == -1))
		cam.directionZ = 0;
	else if (((key == 'a' || key == 'A' || key == 1) && cam.directionX == -1) || ((key == 'd' || key == 'D' || key == 4) && cam.directionX == 1))
		cam.directionX = 0;
}

void Camera::spInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		cam.directionZ = 1;
		break;
	case GLUT_KEY_DOWN:
		cam.directionZ = -1;
		break;
	case GLUT_KEY_LEFT:
		cam.directionX = -1;
		break;
	case GLUT_KEY_RIGHT:
		cam.directionX = 1;
		break;
	default:
		break;
	}
}

void Camera::spInputUP(int key, int x, int y)
{
	if ((key == GLUT_KEY_UP && cam.directionZ == 1) || (key == GLUT_KEY_DOWN && cam.directionZ == -1))
		cam.directionZ = 0;
	else if ((key == GLUT_KEY_LEFT && cam.directionX == -1) || (key == GLUT_KEY_RIGHT && cam.directionX == 1))
		cam.directionX = 0;
}

void Camera::resize(int width, int height)
{
	//Perspect
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, width, height);
	glLoadIdentity();
	gluPerspective(45, double(width / height), 0.1, 1000);

	//Ortho
	glViewport((width / 6) * 5, (height / 4) * 3, width / 6, height / 4);
	glLoadIdentity();
	glOrtho(width / 6, width / 6, height / 4, height / 4, 0.1, 1000);

	//Inventory
	glViewport(0, 0, width, (height / 6));
	glLoadIdentity();
	glOrtho(width, width, height / 6, height / 6, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
}

void Camera::setPerpective()
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, width, height);
	glLoadIdentity();
	gluPerspective(45, double(width / height), 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Camera::setOrthogonal()
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glViewport((width / 6) * 5, (height / 4) * 3, (width / 6), (height / 4));
	glLoadIdentity();
	glOrtho(width / 6, width / 6, height / 4, height / 4, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
}

void Camera::setInventoryCam()
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glViewport(0, 10, width, (height / 6));
	glLoadIdentity();
	glOrtho(width, width, height / 6, height / 6, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
}