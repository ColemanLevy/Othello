//Header file that contains the component that controls the size and placement of all entities
#pragma once

#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component
{
public:
	//The variable that holds the entity's position on screen
	Vector2D position;
	//The height and width of the entity
	int height = 64;
	int width = 64;
	//Scale that is multiplied to the entity's height and width to give the entity's actual size
	int scale = 1;

	//Constructor that sets the entity's initial position to (0, 0) if no position is given
	TransformComponent()
	{
		position.x = 0.0f;
		position.y = 0.0f;
	}
	//If a position is given to the constructor, it sets the entity's initial position to the given position
	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}
};