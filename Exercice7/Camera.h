#pragma once
#include <GL\freeglut_std.h>
#include <vector>
#include "Wall.h"
#include "MiniMap.h"
#include "Key.h"
#include "Maze.h"
#include "Door.h"
#include "Inventory.h"

class Camera
{
private:
	struct DeltaTime
	{
		int oldTime = 0;
		float dt = 0.f;
	};

	struct Vec3
	{
		float x;
		float y;
		float z;
	};
	Vec3 crossProduct(Vec3 vec1, Vec3 vec2)
	{
		float x = (vec1.y * vec2.z) - (vec1.z * vec2.y);
		float y = (vec1.z * vec2.x) - (vec1.x * vec2.z);
		float z = (vec1.x * vec2.y) - (vec1.y * vec2.x);

		return { x, y, z };
	}

	Camera();

	Vec3 vPosition;
	Vec3 vDirection;

	float speed;
	float sensitivity;

	float yaw;
	float pitch;

	int directionX;
	int directionZ;

	float minX;
	float minZ;
	float maxX;
	float maxZ;

	bool crouch;
	float crouchPosition;
	float crouchSpeed;
	float capCrouch;

	bool sprint;

	bool xCollide;
	bool zCollide;
	bool xSlideAxis;

	DeltaTime deltaTime;
	void updateDT();

	MiniMap miniMap;
	Inventory inventory;

	bool showEdge;

	int usedLight;

public:
	static Camera cam;

	static Camera& getInstance();

	void init(float minX, float minZ, float maxX, float maxZ);

	float getDeltaTime();

	void update(std::vector<BoundingBox*>& solidObject, std::vector<Key*> keys, Maze& maze, std::vector<Door*>& doors);

	void activateLight();

	void setLightPos();

	void disableLight();

	void crouchMove();

	void move(std::vector<BoundingBox*>& solidObject, std::vector<Key*> keys, Maze& maze, std::vector<Door*>& doors);

	bool isCollide(float& x, float& z, std::vector<BoundingBox*>& solidObject, std::vector<Key*> keys, Maze& maze, std::vector<Door*>& doors);

	void checkFaceColliding(BoundingBox* obj, float& x, float& z);

	void updateMapCoord();

	MiniMap& getMap();

	Inventory& getInventory();

	static void mouse(int x, int y);

	void inputModifiers();

	static void input(unsigned char key, int x, int y);

	static void inputUP(unsigned char key, int x, int y);

	static void spInput(int key, int x, int y);

	static void spInputUP(int key, int x, int y);

	static void resize(int width, int height);

	void setPerpective();

	void setOrthogonal();

	void setInventoryCam();
};