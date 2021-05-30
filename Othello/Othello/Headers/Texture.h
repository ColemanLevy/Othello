//Header file that contains the TextureManager class which draws and loads all textures in the game
#pragma once

#include "GHeader.h"

class Texture
{
public:
	//Function that loads textures from file path save at pointer
	static SDL_Texture* loadTexture(const char* fileName);
	//Function that draws the given texture at the given size
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
};
