//One of the Component header files that transforms an entity into a button that can be clicked
#pragma once

#include "Components.h"
#include "Vector2D.h"

class ButtonComponent : public Component
{
public:
	//Variable that take's the entity's position variable from its TransformComponent
	//for use in this component
	Vector2D* position;
	//The row and column the button/token is on. Helps with changing the board's values.
	int row = 0;
	int column = 0;
	//The height and width of the button (specified in this case due to the fact there is only
	//one button in the entire game.)
	int height = 64;
	int width = 64;
	//The variables used as the x and y positions of the mouse
	int x, y;
	//The variable that indicates if the button was pressed
	bool press = false;
	//The variable that indicates if the mouse has been clicked while over the button
	bool mouseDown = false;


	//Sets the position of the clickable rectangle for the button to the entity's position
	void init() override
	{
		position = &entity->getComponent<TransformComponent>().position;
	}

	//This update override waits for the left mouse button to be clicked and then 
	//checks to see if the mouse is hovering over the entity and then checks to make
	//sure the button was released while also over the entity. Prevents button spamming
	//from just holding down the mouse button
	void update() override
	{
		//This checks if the mouse button has been clicked
		if (Game::event.type == SDL_MOUSEBUTTONDOWN)
		{
			//Gets the mouse's x and y positions
			SDL_GetMouseState(&x, &y);
			//Checks if the mouse is over the entity
			if ((x > position->x) && (x < position->x + width))
				if ((y > position->y) && (y < position->y + height))
				{
					//If it is, it records that the mouse has been clicked while over the button
					mouseDown = true;
				}
		}
		//Constantly checks if the mouse button has been released
		if (Game::event.type == SDL_MOUSEBUTTONUP)
		{
			//This part of the code only works if the button was clicked on while
			//over the button, helping to prevent any odd issues like being able to
			//release the button and cause any button to work
			if (mouseDown)
			{
				//Gets the mouse's x and y positions
				SDL_GetMouseState(&x, &y);
				//Checks if the mouse is over the entity
				if ((x > position->x) && (x < position->x + width))
					if ((y > position->y) && (y < position->y + height))
					{
						//Changes the bool value to indicate the button has been pressed
						press = true;

						//Saves the row and column over which the mouse was positioned for
						//the board to use
						row = y / 64;
						column = x / 64;
					}
			}
			//Then indicates the mouse is no longer clicked whether or it was released on top of the button 
			mouseDown = false;
		}
	}
};