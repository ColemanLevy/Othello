//Cpp file that contains the functions for what the TextureManager does
#include "Headers/Texture.h"

//Loads texture from sprite pointer
SDL_Texture* Texture::loadTexture(const char* texture)
{
	//Load image from file path saved at pointer
	SDL_Surface* tempSurface = IMG_Load(texture);
	//Turn image into a texture
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	//Open up tempSurface for later use
	SDL_FreeSurface(tempSurface);
	//Return the newly created texture
	return tex;
}

//Small function that draws out the texture the renderer is meant to render
void Texture::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}