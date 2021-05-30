//Header file that holds the Board class and the constructors for its functions and variables
#pragma once
#include "GHeader.h"

class Board
{
public:
	//The constructor function for the board
	Board(int lvl[8][8]);
	//The deconstuctor function for the board
	~Board();

	//The function that updates the board's array
	void LoadBoard(int arr[8][8]);
	//The function that draws the board based on the values in the array
	void DrawBoard();

private:
	//The SDL rectangles used by the board
	SDL_Rect src, dest;
	//The textures used by the board
	SDL_Texture* tile;
	SDL_Texture* tileBlack;
	SDL_Texture* tileWhite;
	SDL_Texture* tileMark;
	//The array used to draw the board on screen
	int board[8][8];
};