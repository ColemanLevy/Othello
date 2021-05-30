//The cpp file that uses the Board.h header to create the board on the screen
#include "Headers/Board.h"
#include "Headers/Texture.h"

//Variable used by the draw program to determine which texture will be drawn on each tile
int type;

//The function that constructs the Board class 
Board::Board(int lvl[8][8])
{
	//Sets the textures for each of the four tiles used on the board
	tile = Texture::loadTexture("Assets/OthelloT(P).png");
	tileBlack = Texture::loadTexture("Assets/OthelloB(P).png");
	tileWhite = Texture::loadTexture("Assets/OthelloW(P).png");
	tileMark = Texture::loadTexture("Assets/OthelloM(P).png");
	//Loads a default map for the map to use
	LoadBoard(lvl);
	//Defines the orientation and how much of the png files is used for each of the textures
	src.x = 0;
	src.y = 0;
	src.h = 64;
	src.w = 64;
	//Defines the orientation and size of each texture used on each tile
	dest.x = 0;
	dest.y = 0;
	dest.w = 64;
	dest.h = 64;
}

//Function that destroys all tile textures on screen, leaving each tile blank and getting
//rid of the recorded textures
Board::~Board()
{
	SDL_DestroyTexture(tile);
	SDL_DestroyTexture(tileBlack);
	SDL_DestroyTexture(tileWhite);
	SDL_DestroyTexture(tileMark);
}

//Updates the multidimensional array used by the board to draw the board on screen with its new values
void Board::LoadBoard(int arr[8][8])
{
	//Increments down each row
	for (int row = 0; row < 8; row++)
	{
		//Increments down each column
		for (int column = 0; column < 8; column++)
		{
			//Updates the section of the array with the value of the inputted array
			board[row][column] = arr[row][column];
		}
	}
}

//Draws the board to the screen using the values from the array
void Board::DrawBoard()
{
	//Initializes the type value to 0
	type = 0;
	//Increments each row
	for (int row = 0; row < 8; row++)
	{
		//Increments each column
		for (int column = 0; column < 8; column++)
		{
			//Sets the tile type to the recorded value 
			type = board[row][column];
			//Sets the x axis position of the tile
			dest.x = column * 64;
			//Sets the y axis position of the tile
			dest.y = row * 64;
			//Draws each tile as based on their type number 
			//(Uses TextureManager's draw function as the tool to draw each tile)
			switch (type)
			{
			case 0:
				//Type 0 tiles are open spots
				Texture::Draw(tile, src, dest);
				break;

			case 1:
				//Type 1 tiles have black tokens
				Texture::Draw(tile, src, dest);
				Texture::Draw(tileBlack, src, dest);
				break;

			case 2:
				//Type 2 tiles have white tokens
				Texture::Draw(tile, src, dest);
				Texture::Draw(tileWhite, src, dest);
				break;

			case 3:
				//Type 3 tiles have the marks for where the player and AI can
				//make their moves
				Texture::Draw(tile, src, dest);
				Texture::Draw(tileMark, src, dest);
				break;

			default:
				break;
			}
		}
	}
}