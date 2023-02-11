#include "pch.h"
#include "Inventory.h"
#include <GL\glew.h>

void Inventory::drawSquare(float r, float g, float b)
{
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.5, 0.5, 0);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(-0.5, 0.5, 0);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.5, -0.5, 0);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(-0.5, -0.5, 0);
	glEnd();
	glColor3f(1, 1, 1);
}

void Inventory::addKey(Key* key)
{
	this->keyObtained.push_back(key);
}

bool Inventory::countainTag(char tag)
{
	for (unsigned int i = 0; i < this->keyObtained.size(); i++)
		if (this->keyObtained[i]->getTag() == tag)
			return true;
	return false;
}

void Inventory::draw(unsigned int keyTexture)
{
	glBindTexture(GL_TEXTURE_2D, keyTexture);
	
	for (unsigned int i = 0; i < this->keyObtained.size(); i++)
	{
		glPushMatrix();

		glTranslatef(-0.1f * (int(this->keyObtained.size() / 2)), 0, 0);
		glTranslatef((i * 0.11f), 0, 0);
		glScalef(0.1f, 1, 1);

		if (this->keyObtained[i]->getTag() == 'a')
			this->drawSquare(1, 1, 0);
		else if (this->keyObtained[i]->getTag() == 'b')
			this->drawSquare(1, 0, 1);
		else if (this->keyObtained[i]->getTag() == 'c')
			this->drawSquare(1, 0.5f, 0);

		glPopMatrix();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
