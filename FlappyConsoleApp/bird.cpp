#pragma once

#include "bird.h"

Bird::Bird()
{
	bird_Y = 0.0;
	velocity = 0.0;
	acceleration = 0.0;
	gravity = 100.0;
	collided = false;
}

Bird::~Bird()
{
	// pusty
}

void Bird::changeVelocity(float v)
{
	velocity = v;
}

float Bird::getVelocity()
{
	return velocity;
}

void Bird::changeAcceleration(float a)
{
	acceleration = a;
}

float Bird::getAcceleration()
{
	return acceleration;
}

void Bird::changeGravity(float g)
{
	gravity = g;
}

float Bird::getGravity()
{
	return gravity;
}

bool Bird::ifColided()
{
	return collided;
}

void Bird::changeColided(bool state)
{
	collided = state;
}

void Bird::changeY(float pos)
{
	bird_Y = pos;
}

float Bird::getY()
{
	return bird_Y;
}

const float Bird::getX()
{
	return bird_X;
}

void Bird::resetBird()
{
	collided = false;
	acceleration = 0.0f;
	velocity = 0.0f;
	bird_Y = SCREEN_HIGHT / 2.0f;
}