#include "Headers/GHeader.h"

Game *game = nullptr;

int main(int argc, const char* argv[])
{
	//The limited frames per second allowed by the window
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	//Variables used to keep track of the ticks in the game, used in the limiting of the FPS
	Uint32 frameStart;
	int frameTime;

	//Creation of the the game and the initializing of the game window
	game = new Game();
	game->init("Project GameBeg", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 832, 640, false);

	//This while loop continues to loop as long as the game is ongoing
	while (game->running())
	{
		//Gets the amount of ticks since the game was started
		frameStart = SDL_GetTicks();

		//Continuously performs these three functions in the game every frame
		game->events();
		game->update();
		game->render();

		//This section limits the frames
		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	//This closes the game
	game->clean();

	//The final return which ends the program
	return 0;
}