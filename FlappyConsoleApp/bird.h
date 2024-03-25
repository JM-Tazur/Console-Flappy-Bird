#pragma once

#include "headers.h"
class Bird
{
private:
	float bird_Y;
	const float bird_X = (int)(SCREEN_WIDTH / 3.0);

	float velocity;
	float acceleration;
	float gravity;
	bool collided;

public:
	Bird();
	~Bird();

	void changeVelocity(float v);
	float getVelocity();
	void changeAcceleration(float a);
	float getAcceleration();
	void changeGravity(float g);
	float getGravity();
	bool ifColided();
	void changeColided(bool state);
	void changeY(float pos);
	float getY();
	const float getX();
	void resetBird();
};