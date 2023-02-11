#pragma once
class BoundingBox
{
private:
	float x;
	float y;
	float z;
	float w;
	float width;
	float height;
	float depth;
public:
	BoundingBox();
	float getX();
	float getY();
	float getZ();
	float getWidth();
	float getHeight();
	float getDepth();
	void setWidth(float width);
	void setHeight(float height);
	void setDepth(float depth);
	void applyTransform();
	const bool operator==(const BoundingBox& box) const;
	virtual void draw() = 0;
	virtual ~BoundingBox() {};
};

