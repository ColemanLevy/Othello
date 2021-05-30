//The header file that contains the class which handles the loading and drawing of all sprites in the game
#pragma once

#include "Components.h"
#include "SDL.h"
#include "Texture.h"

class SpriteComponent : public Component
{
private:
	//Contains the position and size of the entity
	TransformComponent *transform;
	//The variable for texture used for the entity
	SDL_Texture *texture;
	//The SDL rectangles used to size the texture shown
	SDL_Rect srcRect, destRect;

public:
	//The constructor of the sprite component. If a file path is included in the constructor,
	//the entity will be given an initial texture from the image found
	SpriteComponent() = default;
	SpriteComponent(const char* path)
	{
		setTex(path);
	}
	//The deconstructor of the texture, deletes the texture from the entity
	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}
	//A small function that sets the texture of an entity to an image from the file path given
	void setTex(const char* path)
	{
		texture = Texture::loadTexture(path);
	}
	//Sets the position of where the image should be drawn
	void init() override
	{
		//Sets the position of the image to the position of the entity
		transform = &entity->getComponent<TransformComponent>();
		//Indicates how large the image used is and how it should be offset
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = 128;
		srcRect.h = 128;
	}
	//Small function that draws the image to where the entity is and at the size given
	void update() override
	{
		//Position of the entity
		destRect.x = (int)transform->position.x;
		destRect.y = (int)transform->position.y;
		//Size of the texture
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}
	//Draws the actual image to the position specified
	void draw() override
	{
		Texture::Draw(texture, srcRect, destRect);
	}
};