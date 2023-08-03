#pragma once

#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component
{
public:
	Vec2 position;
	Vec2 velocity;

	int height = 16;
	int width = 16;
	int scale = 2;

	int speed = 3;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	TransformComponent(int sc)
	{
		position.x = 400;
		position.y = 320;
		scale = sc;
	}

	TransformComponent(float x, float y, int w, int h, int sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void Init() override
	{
		velocity.Zero();
	}

	void Update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};