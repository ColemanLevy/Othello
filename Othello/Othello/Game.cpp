//-------------------------------------------------------------
//Name: Coleman Levy
//Date: 11/6/2019
//Description: A program that creates a game of Othello that the user can use to play against
//			   another user or play against an AI that successfully use minimax with Alpha-Beta pruning to
//			   make its decisions in the game
//-------------------------------------------------------------
#include "Headers/GHeader.h"
#include "Headers/Texture.h"
#include "Headers/Board.h"
#include "Headers/Components.h"
#include "Headers/Vector2D.h"

//The renderer and event handler used in the game
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

//The board shown in the game
Board* board;

//The manager used by the game to control all objects
Manager manager;

//The multidimensional array used by the game to create the board on the screen.
//This array specifically is what the game uses to show the current board's layout for
//the player and the AI.
int initial[8][8] = {
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,2,1,0,0,0 },
	{ 0,0,0,1,2,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 }
};

//The multidimensional array used for impossible mode, contains a board
//with each tile having "points" associated with it
int impossible[8][8] = {
	{ 4,-3,2,2,2,2,-3,4 },
	{ -3,-4,-1,-1,-1,-1,-4,-3 },
	{ 2,-1,1,0,0,1,-1,2 },
	{ 2,-1,0,1,1,0,-1,2 },
	{ 2,-1,0,1,1,0,-1,2 },
	{ 2,-1,1,0,0,1,-1,2 },
	{ -3,-4,-1,-1,-1,-1,-4,-3 },
	{ 4,-3,2,2,2,2,-3,4 }
};

//Two multidimensional arrays used for calculating changes to the board without
//changing the "initial" board and helps keep the "initial" board consistent
int finality[8][8];
int prophet[8][8];

//The array used by the AI to determine what its list of possible choices are and what
//their score would be
int thought[32][3];

//The button used whenever the player clicks on the board. Uses the row and column
//variables in the board class to determine which tile the player clicked on.
auto& boardButton(manager.addEntity());
//The small square next to the board that shows who is the victor at the end of a game
auto& victoryScr(manager.addEntity());
//The button that starts a new two-player game
auto& startButton(manager.addEntity());
//The button that initiates the debug
auto& debugButton(manager.addEntity());
//The buttons that start a new game with the AI on either easy or hard mode
auto& AIbuttonE(manager.addEntity());
auto& AIbuttonH(manager.addEntity());
auto& AIbuttonI(manager.addEntity());

//Int variables that show the row and column chosen by the AI
int row, col;
//Int variables specifically used by sonar and checker to determine which direction
//the program is looking on the board away from the source token for calculations
int rAdd, cAdd;
//An integer used to help iterate through arrays
int counter;
//The number of white tiles and blakc tiles on the board. Also used by the AI to determine
//scores and what color it is playing as
int whiteT, blackT = 0;
//The integer to indicate how many turns ahead the AI can see
int sight = 2;
//An integer to hold random numbers
int random;
//Integers that hold the change in alpha and beta in the pruning process
int alphaTot, betaTot;
//The integer that records how many branches have been pruned for the debugger
int prunage = 0;

//Boolean that shows whether it's black's turn or white's turn. False = white turn,
//True = black turn
bool bTurn = false;
//Boolean that indicates whether the AI is on or not for the current game
bool AImode = false;
//Boolean that indicates whether the AI should be minimizing or maximizing for the minimax.
//False = max, True = min
bool minimax = false;
//Boolean that indicates whether debugger is on or not
bool debugMode = false;
//Boolean that indicates whether the current game is being saved
bool saveMode = false;
//A boolean used to help with saving. If the AI's turn is first during the
//saving process, it makes it so that the turn will still be saved
bool quickSave = false;
//Boolean that indicates whether the saved game is being loaded
bool loadMode = false;
//Boolean that indicates if impossible mode is on
bool impMode = false;

//The two file streams used for saving and loading the game
ifstream inFile;
ofstream outFile;

//String used for user input
string command;

//The function used to give all of the created game objects their components
void initializer()
{
	//Creates boardButton components
	boardButton.addComponent<TransformComponent>();
	boardButton.addComponent<ButtonComponent>();
	boardButton.getComponent<ButtonComponent>().width = 512;
	boardButton.getComponent<ButtonComponent>().height = 512;

	//Creates victoryScr components
	victoryScr.addComponent<TransformComponent>(512, 0);
	victoryScr.addComponent<SpriteComponent>();
	victoryScr.getComponent<TransformComponent>().height = 512;
	victoryScr.getComponent<TransformComponent>().width = 320;

	//Creates startButton components
	startButton.addComponent<TransformComponent>(0, 576);
	startButton.getComponent<TransformComponent>().height = 64;
	startButton.getComponent<TransformComponent>().width = 128;
	startButton.addComponent<ButtonComponent>();
	startButton.getComponent<ButtonComponent>().height = 64;
	startButton.getComponent<ButtonComponent>().width = 128;
	startButton.addComponent<SpriteComponent>("Assets/StartButt(Oth).png");

	//Creates debugButton components
	debugButton.addComponent<TransformComponent>(150, 576);
	debugButton.getComponent<TransformComponent>().height = 64;
	debugButton.getComponent<TransformComponent>().width = 128;
	debugButton.addComponent<ButtonComponent>();
	debugButton.getComponent<ButtonComponent>().height = 64;
	debugButton.getComponent<ButtonComponent>().width = 128;
	debugButton.addComponent<SpriteComponent>("Assets/DebugButt(Oth).png");

	//Creates AIbuttonE components
	AIbuttonE.addComponent<TransformComponent>(300, 576);
	AIbuttonE.getComponent<TransformComponent>().height = 64;
	AIbuttonE.getComponent<TransformComponent>().width = 128;
	AIbuttonE.addComponent<ButtonComponent>();
	AIbuttonE.getComponent<ButtonComponent>().height = 64;
	AIbuttonE.getComponent<ButtonComponent>().width = 128;
	AIbuttonE.addComponent<SpriteComponent>("Assets/EasyButt(Oth).png");

	//Creates AIbuttonH components
	AIbuttonH.addComponent<TransformComponent>(450, 576);
	AIbuttonH.getComponent<TransformComponent>().height = 64;
	AIbuttonH.getComponent<TransformComponent>().width = 128;
	AIbuttonH.addComponent<ButtonComponent>();
	AIbuttonH.getComponent<ButtonComponent>().height = 64;
	AIbuttonH.getComponent<ButtonComponent>().width = 128;
	AIbuttonH.addComponent<SpriteComponent>("Assets/HardButt(Oth).png");

	//Creates AIbuttonI components
	AIbuttonI.addComponent<TransformComponent>(600, 576);
	AIbuttonI.getComponent<TransformComponent>().height = 64;
	AIbuttonI.getComponent<TransformComponent>().width = 128;
	AIbuttonI.addComponent<ButtonComponent>();
	AIbuttonI.getComponent<ButtonComponent>().height = 64;
	AIbuttonI.getComponent<ButtonComponent>().width = 128;
	AIbuttonI.addComponent<SpriteComponent>("Assets/ImpossButt.png");
}

//The function that writes out all of the debugger statements if debugger is on
//and saves all the important info for a game if the save mode is on
void debugWrite(string write, string save)
{
	//Only writes if debugger is on
	if (debugMode)
		cout << write << endl;
	//Only saves if savemode is on or if quicksave is on
	if (saveMode || quickSave)
	{
		//Doesn't save empty/unimportant info
		if (save != "")
		{
			outFile << save << " " << endl;
		}
	}
}

//The function that ends the current game and calculates the winner if there are no moves left
void gameOver()
{
	//Resets the number of black tiles and number of white tiles
	blackT = 0;
	whiteT = 0;
	//Counts up the number of white and black tiles on the board
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (initial[i][j] == 1)
				blackT++;
			if (initial[i][j] == 2)
				whiteT++;
		}
	//If both tiles are at an even number, the game ends in a tie
	if (blackT == whiteT)
	{
		debugWrite("Game Over! It's a TIE!", "Game Over! It's a TIE!");
		victoryScr.getComponent<SpriteComponent>().setTex("Assets/TieEnd(Oth).png");
	}
	//If there are more black tiles, the game ends in black's victory
	else if (blackT > whiteT)
	{
		debugWrite("Game Over! Black Wins!", "Game Over! Black Wins!");
		victoryScr.getComponent<SpriteComponent>().setTex("Assets/BlackVict(P).png");
	}
	//If there are more white tiles, the game ends in white's victory
	else
	{
		debugWrite("Game Over! White Wins!", "Game Over! White Wins!");
		victoryScr.getComponent<SpriteComponent>().setTex("Assets/WhiteVict(P).png");
	}
	//Turns off AI so that the AI does not try to keep playing
	AImode = false;
	//Turns off save mode and load mode at the end of the game and closes the file they have opened
	if (saveMode)
	{
		saveMode = false;
		cout << "Save mode ended." << endl;
		outFile.close();
	}
	if (loadMode)
	{
		loadMode = false;
		cout << "Load mode ended." << endl;
		inFile.close();
	}
}

//A small function that copies all of the integers in a two-dimensional into another
void copyCat(int in[8][8], int out[8][8])
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			//"IN" receives what is in "OUT"
			in[i][j] = out[i][j];
}

//A small function that clears all marks from the inputted board array
void clearOut(int board[8][8])
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (board[i][j] == 3)
				board[i][j] = 0;
}

//Fuction used to make sure sonar's calculations do not go past the edges of the board
bool checker(int row, int col, int rowAdd, int colAdd)
{
	//If sonar is looking up, stop if the current tile is at the top edge of the board
	if (rowAdd > 0 && colAdd == 0)
	{
		//If the current tile is not at the top edge, keep going
		if (row - rowAdd != 0)
			return true;
		else
			return false;
	}
	//If sonar is looking down, stop if the current tile is at the bottom edge of the board
	else if (rowAdd < 0 && colAdd == 0)
	{
		//If the current tile is not at the bottom edge, keep going
		if (row - rowAdd != 7)
			return true;
		else
			return false;
	}
	//If sonar is looking to the left, stop if the current tile is at the left edge of the board
	else if (rowAdd == 0 && colAdd > 0)
	{
		//If the current tile is not at the left edge, keep going
		if (col - colAdd != 0)
			return true;
		else
			return false;
	}
	//If sonar is looking right, stop if the current tile is at the right edge of the board
	else if (rowAdd == 0 && colAdd < 0)
	{
		//If the current tile is not at the bottom edge, keep going
		if (col - colAdd != 7)
			return true;
		else
			return false;
	}
	//If sonar is looking to the upper-left, stop if the current tile is at the left edge or top edge of the board
	else if (rowAdd > 0 && colAdd > 0)
	{
		//If the current tile is not at the left edge or top edge, keep going
		if (row - rowAdd != 0 && col - colAdd != 0)
			return true;
		else
			return false;
	}
	//If sonar is looking to the lower-left, stop if the current tile is at the left edge or bottom edge of the board
	else if (rowAdd < 0 && colAdd > 0)
	{
		//If the current tile is not at the left edge or bottom edge, keep going
		if (row - rowAdd != 7 && col - colAdd != 0)
			return true;
		else
			return false;
	}
	//If sonar is looking to the upper-right, stop if the current tile is at the right edge or top edge of the board
	else if (rowAdd > 0 && colAdd < 0)
	{
		//If the current tile is not at the right edge or top edge, keep going
		if (row - rowAdd != 0 && col - colAdd != 7)
			return true;
		else
			return false;
	}
	//If sonar is looking to the lower-right, stop if the current tile is at the right edge or bottom edge of the board
	else if (rowAdd < 0 && colAdd < 0)
	{
		//If the current tile is not at the right edge or bottom edge, keep going
		if (row - rowAdd != 7 && col - colAdd != 7)
			return true;
		else
			return false;
	}
	else
		return false;
}

//Function that the AI uses for the Alpha-Beta pruning
bool AIchecker(bool decide, int alpha, int beta, int test)
{
	//If the current layer is maximizing
	if (!decide)
	{
		//And if the current branch has a score lower than the beta
		if (test <= beta)
		{
			//Proceed with minimax search
			return true;
		}
		else
		{
			//Otherwise, increase the number of branches pruned and don't search any deeper
			prunage++;
			return false;
		}
	}
	//If the current layer is minimizing
	else
	{
		//And if the current branch has a score higher than the alpha
		if (test >= alpha)
		{
			//Proceed with minimax search
			return true;
		}
		else
		{
			//Otherwise, increase the number of branches pruned and don't search any deeper
			prunage++;
			return false;
		}
	}
}

//The function that performs the calculations for flipping tokens and adding marks on the board
void sonar(int row, int col, int color, int rowAdd, int colAdd, bool mode, int board[8][8])
{
	//The direction the sonar follows away from the source token inputted
	rAdd = rowAdd;
	cAdd = colAdd;
	//If the token seen next to the source token in the direction given is the opposite color of the source token,
	//proceed to search in that direction
	if (board[row - rAdd][col - cAdd] == ((color - 3) * (-1)))
	{
		//While the tokens being found are of the opposite color of the source token and the search hasn't
		//hit the edge of the board, continue searching in that direction
		while (board[row - rAdd][col - cAdd] == ((color - 3) * (-1)) && checker(row, col, rAdd, cAdd))
		{
			rAdd += rowAdd;
			cAdd += colAdd;
		}
		//Once at the end of the line of enemy tokens, make a decision based on the mode. If true,
		//proceed to calculate token flips
		if (mode)
		{
			//If the line of enemy tokens ends in an ally token (a token of the same color as
			//the source token), proceed to go back towards the source token
			if (board[row - rAdd][col - cAdd] == color)
			{
				rAdd -= rowAdd;
				cAdd -= colAdd;
				//While going back to the source token, flip all enemy tokens along the way
				//until the search has arrived back at the source token
				while (board[row - rAdd][col - cAdd] == ((color - 3) * (-1)))
				{
					//Changes the calculation board so as to not change the current board which is
					//being used (prevents changes from the current board until all calculations
					//are complete)
					finality[row - rAdd][col - cAdd] = color;
					rAdd -= rowAdd;
					cAdd -= colAdd;
				}
			}
		}
		//If mode is false, calculate where to place marks
		else
		{
			//If the end of the line of enemy tokens is an open space, place a mark
			if (board[row - rAdd][col - cAdd] == 0)
			{
				//Just like in Board.cpp, tiles with a 3 contain a viable move mark
				//Changes the calculation board so as to not change the current board which is
				//being used (prevents changes from the current board until all calculations
				//are complete)
				finality[row - rAdd][col - cAdd] = 3;
			}
		}
	}
	//Reset the direction of the search
	rAdd = 0;
	cAdd = 0;
}

//The function that calls sonar and gives the direction in which the sonar should search
//from the source token (aka the point where the player/AI just put their token)
void ping(int row, int col, int color, bool mode, int board[8][8])
{
	//If not at the top edge of the board...
	if (row != 0)
	{
		//Do a sonar search upwards
		sonar(row, col, color, 1, 0, mode, board);
	}
	//If not at the bottom edge of the board...
	if (row != 7)
	{
		//Do a sonar search downwards
		sonar(row, col, color, -1, 0, mode, board);
	}
	//If not at the left edge of the board...
	if (col != 0)
	{
		//Do a sonar search to the left
		sonar(row, col, color, 0, 1, mode, board);
	}
	//If not at the right edge of the board...
	if (col != 7)
	{
		//Do a sonar search to the right
		sonar(row, col, color, 0, -1, mode, board);
	}
	//If not at the top edge or left edge of the board...
	if (row != 0 && col != 0)
	{
		//Do a sonar search to the upper left
		sonar(row, col, color, 1, 1, mode, board);
	}
	//If not at the bottom edge or left edge of the board...
	if (row != 7 && col != 0)
	{
		//Do a sonar search to the lower left
		sonar(row, col, color, -1, 1, mode, board);
	}
	//If not at the top edge or right edge of the board...
	if (row != 0 && col != 7)
	{
		//Do a sonar search to the upper right
		sonar(row, col, color, 1, -1, mode, board);
	}
	//If not at the bottom edge or right edge of the board...
	if (row != 7 && col != 7)
	{
		//Do a sonar search to the lower right
		sonar(row, col, color, -1, -1, mode, board);
	}
}

//Small function that pauses rendering momentarily. Use to show the AI taking its turn
//or when a character's turn is skipped so that the user can realize this has occured
void Game::pause()
{
	//Refreshes the manager and performs any updates the manager has to perform (mostly
	//just for entity deletion)
	manager.refresh();
	manager.update();
	//Loads the current board to the board class used in the game
	board->LoadBoard(initial);
	//Renders all graphics on screen
	render();
	//This causes the code to wait for half a second
	this_thread::sleep_for(chrono::milliseconds(500));
}

//The function called whenever the player up next (whether it is AI or a user) has no
//available viable moves. It skips that player's turn and then check to see if the other
//player has any viable moves. If not, the game concludes.
void turnLite()
{
	//counter is reset
	counter = 0;
	//Goes to the next player's turn (the player after the skipped player)
	bTurn = !bTurn;
	//Clears all marks off the board
	clearOut(initial);
	//If it is the black player's turn, it sets all the marks on the board for that player
	if (bTurn)
	{
		//Copies the current board to the board used for calculations
		copyCat(finality, initial);
		//Find all viable moves for player black
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (initial[i][j] == 1)
				{
					ping(i, j, 1, false, initial);
				}
		//Updates the current board with all the added marks
		copyCat(initial, finality);
	}
	//If it is the white player's turn, it sets all the marks on the board for that player
	else
	{
		//Copies the current board to the board used for calculations
		copyCat(finality, initial);
		//Find all viable moves for player white
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (initial[i][j] == 2)
				{
					ping(i, j, 2, false, initial);
				}
		//Updates the current board with all the added marks
		copyCat(initial, finality);
	}
	//Counts up the total number of marks to see if the player
	//has any viable moves
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (initial[i][j] == 3)
				counter++;
	//If there are none, the game concludes
	if (counter == 0)
		gameOver();
	//If there are some, the counter is set to -1 which is used
	//for skipping the AI's turn, in case it is on
	else
		counter = -1;
}

//The function that performs all token flips and mark placements for the next player
//and then sets it to be their turn. This takes in the parameter of what tile the
//previous player chose (whether it is AI or user)
void Game::nextTurn(int row, int col)
{
	//Resets the iterator
	counter = 0;
	//Sets it to be the next player's turn
	bTurn = !bTurn;
	//Clears all marks off the board
	clearOut(initial);

	//If it is player black's turn, set up the board for them
	if (bTurn)
	{
		//Copy the current board to the calculation board
		copyCat(finality, initial);
		//Do the flips for the white player's move that they just performed
		ping(row, col, 2, true, initial);
		//Set changes to the current board
		copyCat(initial, finality);
		//Mark all the viable moves for player black
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (initial[i][j] == 1)
				{
					ping(i, j, 1, false, initial);
				}
		//Save those marks to the current board
		copyCat(initial, finality);
	}
	//If it is player white's turn, set up the board for them
	else
	{
		//Copy the current board to the calculation board
		copyCat(finality, initial);
		//Do the flips for the black player's move that they just performed
		ping(row, col, 1, true, initial);
		//Set changes to the current board
		copyCat(initial, finality);
		//Mark all the viable moves for player white
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (initial[i][j] == 2)
				{
					ping(i, j, 2, false, initial);
				}
		//Save those marks to the current board
		copyCat(initial, finality);
	}
	//Check if there are any viable moves for the player whos turn has just begun
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (initial[i][j] == 3)
				counter++;
	//If there are no viable moves, skip their turn
	if (counter == 0)
	{
		pause();
		turnLite();
	}
}

//The function used to recursively see further into future turns for the AI
int foresee(int color, int next[8][8], int seeing, bool decide, int alph, int bet)
{
	//Creates a small array to store scores so as to create a search tree
	int think[32];
	//Variable meant to help with calculating if the current decision leads to a dead end
	int cutoff = seeing;
	//If this layer is minimizing, set the node's current value to something
	//that is very high so that it will be instantly overriden by the next calculated score
	if (decide)
	{
		for (int i = 0; i < 32; i++)
			think[i] = 100;
	}
	//If this layer is maximizing, set the node's current value to something
	//that is very low so that it will be instantly overriden by the next calculated score
	else
	{
		for (int i = 0; i < 32; i++)
			think[i] = -100;
	}
	
	//Create an iterator for this node to save its place in its array of scores
	int countUp = 0;
	//Create the alpha and beta for this node for the Alpha-Beta pruning
	int alpha = alph;
	int beta = bet;
	//Copy all the values from the parent node's board to the
	//future board state
	copyCat(prophet, next);
	//Copy all values from the future board to the calculation board
	copyCat(finality, prophet);
	//Mark all the viable moves on the future board
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (prophet[i][j] == color)
			{
				ping(i, j, color, false, prophet);
			}
	//Add said marks to the future board
	copyCat(prophet, finality);
	//Reset score
	whiteT = 0;
	//Counts up the number of available moves, and if there are no, sets it so
	//the tree doesn't attempt to look further and just calculates score
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (prophet[i][j] == 3)
			{
				whiteT++;
			}
	//If there are no more moves to make, don't look deeper
	if (whiteT <= 0)
	{
		cutoff = -1;
	}
	//If allowed to look deeper...
	if (cutoff > -1)
	{
		//For all possible viable moves, calculate the score of performing these moves
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (prophet[i][j] == 3)
				{
					//Set a token of the AI's color on the future board
					prophet[i][j] = color;
					//Get rid of all marks on the future board
					clearOut(prophet);
					//Copy the future board to the calculation board to make them equal
					copyCat(finality, prophet);
					//Performs the token flips caused by this move
					ping(i, j, color, true, prophet);
					//Flips the tokens on the future board by copying the values
					//from the calculation board
					copyCat(prophet, finality);
					//If the current node is not the deepest possible leaf node, attempt to
					//look further into the future based on the move that was just made
					if (seeing > 0)
					{
						//If no scores have been calculated for this node, look further into the future
						if (countUp == 0)
						{
							//Creates a temporary board so as to make a unchanging paramet for the 
							//recursive foresee function
							int remember[8][8];
							//Sets the temporary board to be the same as the future board
							copyCat(remember, prophet);
							//Recursively call foresee using the temporary board as input and setting the player to be
							//the next player, setting the minimax to be the next layer, lowering the number of layers
							//the AI can see further, and sending in the current alpha and beta values. Add the score
							//that comes from this search to the list of scores
							think[countUp] = foresee(((color - 3) * (-1)), remember, seeing - 1, !decide, alpha, beta);
							//If this layer is minimizing, set beta to equal the alpha from the child node that was just
							//calculated if the child node's alpha was lower than the current beta
							if (decide)
							{
								beta = min(betaTot, beta);
							}
							//If this layer is maximizing, set alpha to equal the beta from the child node that was just
							//calculated if the child node's beta was higher than the current alpha
							else
							{
								alpha = max(alphaTot, alpha);
							}
						}
						//If a previous score has been calculated...
						else
						{
							//Decide whether to continue searching deeper or prune this branch
							//based on the result from the AIchecker
							if (AIchecker(decide, alpha, beta, think[countUp - 1]))
							{
								//Creates a temporary board so as to make a unchanging paramet for the 
								//recursive foresee function
								int remember[8][8];
								//Sets the temporary board to be the same as the future board
								copyCat(remember, prophet);
								//Recursively call foresee using the temporary board as input and setting the player to be
								//the next player, setting the minimax to be the next layer, lowering the number of layers
								//the AI can see further, and sending in the current alpha and beta values Add the score
								//that comes from this search to the list of scores
								think[countUp] = foresee(((color - 3) * (-1)), remember, seeing - 1, !decide, alpha, beta);
								//If this layer is minimizing, set beta to equal the alpha from the child node that was just
								//calculated if the child node's alpha was lower than the current beta
								if (decide)
								{
									beta = min(betaTot, beta);
								}
								//If this layer is maximizing, set alpha to equal the beta from the child node that was just
								//calculated if the child node's beta was higher than the current alpha
								else
								{
									alpha = max(alphaTot, alpha);
								}
							}
						}
					}
					//If this next node IS the deepest possible leaf node, then try to calculate the score of this node
					else
					{
						//If no scores have been calculated for this node, calculate the score
						if (countUp == 0)
						{
							//Reset the number of AI color tokens recorded
							whiteT = 0;
							//If Impossible mode is on, use the Impossible scoreboard to add points
							if (impMode)
							{
								for (int k = 0; k < 8; k++)
									for (int l = 0; l < 8; l++)
										if (prophet[k][l] == blackT)
											whiteT += impossible[k][l];
							}
							//Else, for each AI token on the board, add 1 to the score of this node
							else
							{
								for (int k = 0; k < 8; k++)
									for (int l = 0; l < 8; l++)
										if (prophet[k][l] == blackT)
											whiteT++;
							}
							//Add the score of the leaf node to the list of scores
							think[countUp] = whiteT;
							//If this layer is minimizing, then check if the score is less than
							//the current beta. If so, set the current beta to said score
							if (decide)
							{
								beta = min(whiteT, beta);
							}
							//If this layer is maximizing, then check if the score is greater than
							//the current alpha. If so, set the current alpha to said score
							else
							{
								alpha = max(whiteT, alpha);
							}
						}
						//If a previous score has been calculated...
						else
						{
							//Decide whether to calculate the score or prune this branch
							//based on the result from the AIchecker
							if (AIchecker(decide, alpha, beta, think[countUp - 1]))
							{
								//Reset the number of AI color tokens recorded
								whiteT = 0;
								//If Impossible mode is on, use the Impossible scoreboard to add points
								if (impMode)
								{
									for (int k = 0; k < 8; k++)
										for (int l = 0; l < 8; l++)
											if (prophet[k][l] == blackT)
												whiteT += impossible[k][l];
								}
								//Else, for each AI token on the board, add 1 to the score of this node
								else
								{
									for (int k = 0; k < 8; k++)
										for (int l = 0; l < 8; l++)
											if (prophet[k][l] == blackT)
												whiteT++;
								}
								//Add the score of the leaf node to the list of scores
								think[countUp] = whiteT;
								//If this layer is minimizing, then check if the score is less than
								//the current beta. If so, set the current beta to said score
								if (decide)
								{
									beta = min(whiteT, beta);
								}
								//If this layer is maximizing, then check if the score is greater than
								//the current alpha. If so, set the current alpha to said score
								else
								{
									alpha = max(whiteT, alpha);
								}
							}
						}
					}
					//Reset future board to be the same as the inputted board to
					//undo the turn that was made
					copyCat(prophet, next);
					//Copy the future board to the calculation board
					copyCat(finality, prophet);
					//Mark all the viable moves on the future board
					for (int k = 0; k < 8; k++)
						for (int l = 0; l < 8; l++)
							if (prophet[k][l] == color)
							{
								ping(k, l, color, false, prophet);
							}
					//Set those viable moves on the future board
					copyCat(prophet, finality);
					//Increment the iterator
					countUp++;
				}
	}
	//Othewise, count up the score of the current state and instead return that
	else
	{
		//Reset the number of AI color tokens recorded
		whiteT = 0;
		//If Impossible mode is on, use the Impossible scoreboard to add points
		if (impMode)
		{
			for (int k = 0; k < 8; k++)
				for (int l = 0; l < 8; l++)
					if (prophet[k][l] == blackT)
						whiteT += impossible[k][l];
		}
		//Else, for each AI token on the board, add 1 to the score of this node
		else
		{
			for (int k = 0; k < 8; k++)
				for (int l = 0; l < 8; l++)
					if (prophet[k][l] == blackT)
						whiteT++;
		}
		//Add the score of the leaf node to the list of scores
		think[countUp] = whiteT;
		//If this layer is minimizing, then check if the score is less than
		//the current beta. If so, set the current beta to said score
		if (decide)
		{
			beta = min(whiteT, beta);
		}
		//If this layer is maximizing, then check if the score is greater than
		//the current alpha. If so, set the current alpha to said score
		else
		{
			alpha = max(whiteT, alpha);
		}
	}
	//Once all possible options have been explored, set the score to
	//be a value the will instantly be overriden by the first score
	//of the node
	if (decide)
	{
		whiteT = 100;
	}
	else
	{
		whiteT = -100;
	}
	//If the node is minimizing, take the lowest score...
	if (decide)
	{
		for (int i = 0; i < 32; i++)
		{
			if (think[i] < whiteT)
			{
				whiteT = think[i];
			}
		}
	}
	//If the node is maximizing, take the highest score...
	else
	{
		for (int i = 0; i < 32; i++)
		{
			if (think[i] > whiteT)
			{
				whiteT = think[i];
			}
		}
	}
	//If the node is minimizing, send the beta value to the parent node
	if (decide)
	{
		alphaTot = beta;
	}
	//If the node is maximizing, send the alpha value to the parent node
	else
	{
		betaTot = alpha;
	}
	//And return the decided upon score
	return whiteT;
}

//The function that lets the AI read the current board and begin to make its
//calculations (from the Alpha-Beta minimax search)
void Game::AIturn(int seeing)
{
	//Reset the number of branches pruned to zero
	prunage = 0;
	//If the debugger is on, write that it is the AI's turn
	debugWrite("AI turn, it's choices are:", "");
	//Creates a temporary board so as to make a unchanging paramet for the 
	//recursive foresee function
	int remember[8][8];
	//Reset the alpha and beta that are sent up to the parnt node of whatever is
	//the current node at the time
	alphaTot = 0;
	betaTot = 0;
	//Reset the alpha and beta for the root node
	int alpha = -100;
	int beta = 100;
	//Reset the iterator for the root node
	counter = 0;
	//Reset the score
	whiteT = 0;
	//Create an integer that stores what the level of sight the
	//AI has
	int believing = seeing;
	//If it is black turn on AI turn, set AI to be player black. Otherwise,
	//set AI to be player white.
	if (bTurn)
		blackT = 1;
	else
		blackT = 2;
	//Sets minimax boolean to false to show current layer is maximizing
	minimax = false;
	//Pauses for the user to see the AI thinking about its turn
	pause();
	//Sets the list of scores in the root node to be values that will
	//be instantly overriden by other scores.
	for (int i = 0; i < 32; i++)
		thought[i][0] = -100;
	//Calculates the number of possible future viable moves for both players
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (initial[i][j] == 3 || initial[i][j] == 0)
				whiteT++;
	//If there are less moves than there is number of layers deep that the AI
	//can see, reduce the number of visible layers for the AI to be equal to
	//the total number of available moves
	if (whiteT - 1 < believing)
		believing = whiteT - 1;
	//Reset score
	whiteT = 0;
	//Copy current board to future board
	copyCat(prophet, initial);
	//For all viable moves, play the move on the future board and
	//calculate the score from said move
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (prophet[i][j] == 3)
			{
				//Set the future board mark found to a token of the AI's color
				prophet[i][j] = blackT;
				//Clear all marks from the future board
				clearOut(prophet);
				//Saves changes to the future board
				copyCat(finality, prophet);
				//Performs the token flips caused by this move
				ping(i, j, blackT, true, prophet);
				//Saves changes to future board
				copyCat(prophet, finality);
				//Saves future board to temporary board
				copyCat(remember, prophet);
				//If there is more than one move available, proceed to start minimax search
				if (believing > 0)
					thought[counter][0] = foresee(((blackT - 3) * (-1)), remember, believing -1, true, alpha, beta);
				//If there is only one move left, calculate the score of that one move
				else
				{
					//For each token of the AI's color, add 1 to score
					for (int k = 0; k < 8; k++)
						for (int l = 0; l < 8; l++)
							if (prophet[k][l] == blackT)
								whiteT++;
					//Add said score to the list
					thought[counter][0] = whiteT;
				}
				//If the beta from the leaf child node is greater than the current
				//alpha, set the root node's alpha to the be child node's beta
				if (alphaTot > alpha)
					alpha = alphaTot;
				//Record the tile of the move that was just performed
				thought[counter][1] = i;
				thought[counter][2] = j;
				//Reset future board to be equal to current board
				copyCat(prophet, initial);
				//Increment the iterator
				counter++;
			}
	//Reset the iterator
	counter = 0;
	//Set whiteT to be a value that will be instantly overriden by any score
	whiteT = -100;
	//Choose the highest score from the list and record the move that ends with that score
	for (int i = 0; i < 32; i++)
	{
		if (thought[i][0] > whiteT)
		{
			counter = i;
			whiteT = thought[i][0];
		}
	}
	//Set the AI's choice move to be the highest score's move
	row = thought[counter][1];
	col = thought[counter][2];
	//Set a token of the AI's color on the chosen tile on the board
	initial[row][col] = blackT;
	//For all choices and scores in the list that have been calculated, write out the moves and scores to the debugger
	for (int i = 0; i < 32; i++)
		if (thought[i][0] > -100)
			debugWrite("Row: " + std::to_string(thought[i][1] + 1) + "  Column: " + std::to_string(thought[i][2] + 1) +
				"  Score: " + std::to_string(thought[i][0]), "");
	//Write out the move the AI has chosen to the debugger
	debugWrite("AI has chosen " + std::to_string(row + 1) + ":" + std::to_string(col + 1), "AI turn " + std::to_string(row) + " " + std::to_string(col));
	//Write out the number of branches pruned to the debugger
	debugWrite("Branches pruned: " + std::to_string(prunage), "");
	//Perform the next turn function
	nextTurn(row, col);
}

//The function that resets the board for a new game (also turns on AI and sets
//its difficulty if the AI buttons are pressed)
void Game::newGame(int vision, bool AI, bool impBool)
{
	//Write to a debugger that a new game has started without saving this info
	debugWrite("\nNew Game Started", "");
	//If save mode is on, turns it off and closes the file
	if (saveMode)
	{
		saveMode = false;
		cout << "Save mode ended." << endl;
		outFile.close();
	}
	//If load mode is on, turns it off and closes the files
	if (loadMode)
	{
		loadMode = false;
		cout << "Load mode ended." << endl;
		inFile.close();
	}
	//Resets the board to its initial position for every game
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			initial[i][j] = 0;
	initial[3][3] = 2;
	initial[4][3] = 1;
	initial[3][4] = 1;
	initial[4][4] = 2;
	//The victory screen sprite is turned off whether or not it is on
	victoryScr.getComponent<SpriteComponent>().setTex("");
	//Sets game to player black's turn since player black always goes first
	bTurn = true;
	//Clears all marks from the board
	clearOut(initial);
	//Copies current board to calculation board
	copyCat(finality, initial);
	//Marks all viable moves on the board for player black
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (initial[i][j] == 1)
			{
				ping(i, j, 1, false, initial);
			}
	//Saves changes to current board
	copyCat(initial, finality);
	//Sets AI mode and AI difficulty based on which button was pressed
	AImode = AI;
	sight = vision;
	impMode = impBool;
	//Does the coin flip for which player goes first
	random = rand() % 2 + 1;
	if (AImode)
		//If AI is on and wins coin flip, AI goes first
		if (random == 1)
			AIturn(sight - 1);
		//Otherwise, if player wins coin flip, player goes first and writes said detail
		//to the debugger
		else
			debugWrite("Player Turn.", "");
}

//Function that controls whether save mode or load mode are turned on. User inputs are done
//in the command line window
void Game::LScontrols()
{
	//If debugmode is on, proceeds to ask if user wants to use save mode or load mode
	if (debugMode)
	{
		//Asks user if they wish to use save mode
		cout << "Do you wish to save the next game? \nThis will start a new game on hard mode and override the previous saved game."
			<< "Type Y/N." << endl;
		//Resets command
		command = "";
		//Takes user input
		cin >> command;
		//Sets user input to lower case
		transform(command.begin(), command.end(), command.begin(), ::tolower);
		//If user says yes, turns on save mode
		if (command == "y")
		{
			//Performs initial actions to turn on save mode such as starting the new game and opening the save file
			cout << "Saving turned on. Turning debug off, starting a new game, and the end of the current game will turn off saving." << endl;
			outFile.open("Save.txt");
			quickSave = true;
			//Starts a new game with AI on hard mode
			newGame(4, true, false);
			quickSave = false;
			saveMode = true;
		}
		//If the user says no, save mode stays off and the user is then asked if they wish to turn
		//on load mode instead
		else if (command == "n")
		{
			//Informs user save mode was left off
			cout << "Save mode left off." << endl;
			//Asks user if they wish to use load mode
			cout << "Do you wish to load the saved game? This will cause the entire saved game to play out."
				<< "Type Y/N" << endl;
			//Resets command
			command = "";
			//Takes user input
			cin >> command;
			//Sets user input to lower case
			transform(command.begin(), command.end(), command.begin(), ::tolower);
			//If user says yes, turns on save mode
			if (command == "y")
			{
				//Performs initial actions to turn on load mode such as starting the new game and opening the save file
				newGame(0, false, false);
				loadMode = true;
				inFile.open("Save.txt");
				cout << "Load mode on. Click on board to load the next turn in the save. \nStarting a new game, turning off "
					<< "Debug mode, and the game ending will turn off load mode" << endl;
			}
			//If the user says no, load mode stays off
			else if (command == "n")
			{
				cout << "Load mode left off." << endl;
			}
			//If the user's input isn't recognized, load mode stays off
			else
			{
				cout << "Input unknown, Load mode left off." << endl;
			}
		}
		//If the user's input isn't recognized, save mode stays off and the user is then asked if they wish to turn
		//on load mode instead
		else
		{
			//Informs user save mode was left off
			cout << "Input unknown, Save mode left off." << endl;
			//Asks user if they wish to use load mode
			cout << "Do you wish to load the saved game? This will cause the entire saved game to play out. This will also turn off Save mode."
				<< "Type Y/N" << endl;
			//Resets command
			command = "";
			//Takes user input
			cin >> command;
			//Sets user input to lower case
			transform(command.begin(), command.end(), command.begin(), ::tolower);
			if (command == "y")
			{
				//Performs initial actions to turn on save mode such as starting the new game and opening the save file
				newGame(0, false, false);
				loadMode = true;
				cout << "Load mode on. Click on board to load the next turn in the save. \nStarting a new game or turning off "
					<< "Debug mode will turn off load mode" << endl;
			}
			//If the user says no, load mode stays off
			else if (command == "n")
			{
				cout << "Load mode left off." << endl;
			}
			//If the user's input isn't recognized, load mode stays off
			else
			{
				cout << "Input unknown, Load mode left off." << endl;
			}
		}
	}
	//If the debugger is being turned off, turns off save mode and load mode and closes
	//their files
	else
	{
		if (saveMode)
		{
			saveMode = false;
			cout << "Save mode ended." << endl;
			outFile.close();
		}
		if (loadMode)
		{
			loadMode = false;
			cout << "Load mode ended." << endl;
			inFile.close();
		}
	}
}

//Function that controls what each of the buttons do in the game when they are pressed
void Game::buttonControl()
{
	//If the boardButton is pressed...
	if (boardButton.getComponent<ButtonComponent>().press)
	{
		//If load mode is on, proceeds to read all the saved data in the save file
		if (loadMode)
		{
			//Reading saved data 
			inFile >> command;
			//If the save file isn't empty...
			if (command != "")
			{
				//Reading saved data and write out who's turn it is
				cout << command << " ";
				inFile >> command;
				cout << command << endl;
				//Takes the move that was played by the saved player
				inFile >> row;
				inFile >> col;
				//If the player is player black, then uses a black token
				//on the loaded tile
				if (bTurn)
				{
					initial[row][col] = 1;
				}
				//If the player is player white, then uses a white token
				//on the loaded tile
				else
				{
					initial[row][col] = 2;
				}
				//Performs the nextTurn function for the player that just went
				nextTurn(row, col);
			}
			//Turns off boardButton to prevent infinite spamming of clicks
			boardButton.getComponent<ButtonComponent>().press = false;
		}
		//If load mode is't on, perfroms the player's turn
		else
		{
			//Records which tile the player clicked upon
			row = boardButton.getComponent<ButtonComponent>().row;
			col = boardButton.getComponent<ButtonComponent>().column;
			//If the player chose a viable move...
			if (initial[row][col] == 3)
			{
				//If the player is player black, then a black token is placed
				//at the chosen tile
				if (bTurn)
				{
					initial[row][col] = 1;
				}
				//If the player is player white, then a white token is placed
				//at the chosen tile
				else
				{
					initial[row][col] = 2;
				}
				//Write to the debugger what move the player just made
				debugWrite("Player has chosen " + std::to_string(row + 1) + ":" + std::to_string(col + 1), "Player Turn " + 
					std::to_string(row) + " " + std::to_string(col));
				//Performs the nextTurn function for the player who just went
				nextTurn(row, col);
				//If AI is turned on, performs the AI's turn
				if (AImode)
				{
					//Unless the AI's turn was skipped
					if (counter >= 0)
						AIturn(sight - 1);
					else
					{
						//Writes that the AI's turn was skipped to the debugger
						debugWrite("AI turn SKIPPED", "");
						//Resets the counter value to indicate the AI's skipped turn
						//was completed
						counter = 0;
					}
					//If after the AI's turn is completed and the game is indicating at turn
					//should be skipped, the player's turn is skipped
					if (counter < 0)
					{
						//The player's turn is skipped until the counter indicates the
						//player has a turn once more
						while (counter < 0)
						{
							debugWrite("Player turn SKIPPED", "");
							AIturn(sight - 1);
						}
					}
				}
				//Turns off boardButton to prevent infinite spamming of clicks
				boardButton.getComponent<ButtonComponent>().press = false;
			}
			//Otherwise, if the tile chosen was not a valid move...
			else
				//Turns off boardButton to prevent infinite spamming of clicks
				boardButton.getComponent<ButtonComponent>().press = false;
		}
	}

	//If the startButton is clicked, starts a new two player game
	if (startButton.getComponent<ButtonComponent>().press)
	{
		newGame(0, false, false);
		//Turns off startButton to prevent infinite spamming of clicks
		startButton.getComponent<ButtonComponent>().press = false;
	}

	//If debugButton is clicked...
	if (debugButton.getComponent<ButtonComponent>().press)
	{
		//Writes whether the Debugger was just turned off or on
		if (debugMode)
			cout << "Debug mode: OFF" << endl;
		else
			cout << "Debug mode: ON" << endl;
		//Sets debugger to off/on based on what it was previously
		debugMode = !debugMode;
		//Performs the function for checking if the user wishes to save
		//or load
		LScontrols();
		//Turns off debugButton to prevent infinite spamming of clicks
		debugButton.getComponent<ButtonComponent>().press = false;
	}

	//If the easy AIbutton was clicked, starts a new game with AI on easy mode
	if (AIbuttonE.getComponent<ButtonComponent>().press)
	{
		debugWrite("AI: EASY mode", "");
		newGame(2, true, false);
		//Turns off easy AIButton to prevent infinite spamming of clicks
		AIbuttonE.getComponent<ButtonComponent>().press = false;
	}

	//If the hard AIbutton was clicked, starts a new game with AI on hard mode
	if (AIbuttonH.getComponent<ButtonComponent>().press)
	{
		debugWrite("AI: HARD mode", "");
		newGame(4, true, false);
		//Turns off hard AIButton to prevent infinite spamming of clicks
		AIbuttonH.getComponent<ButtonComponent>().press = false;
	}

	//If the impossible AIbutton was clicked, starts a new game with AI on impossible mode
	if (AIbuttonI.getComponent<ButtonComponent>().press)
	{
		debugWrite("AI: IMPOSSIBLE mode", "");
		newGame(5, true, true);
		//Turns off impossible AIButton to prevent infinite spamming of clicks
		AIbuttonI.getComponent<ButtonComponent>().press = false;
	}
}

//Function that initializes game and the window the game is played in
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	//Value used for the game's fullscreen mode. Currently unused.
	int flags = 0;
	//if (fullscreen)
	//	flags = SDL_WINDOW_FULLSCREEN;

	//Small function that initializes the window and draws the background onto it. Comes with
	//small messages to indicate the progress of the window's creation
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		cout << "Initialized..." << endl;
		//Creates the actual window
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
			cout << "Window created." << endl;
		//Renders what is in the windown
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			//Sets the background of the windown to white
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			cout << "Renderer created." << endl;
		}
		//Finally, shows the game is operational by setting "isRunning" to true
		isRunning = true;

		//Creates a new board
		board = new Board(initial);
		//Creates all the components for the game objects
		initializer();
		//Starts a new game on two player mode
		newGame(0, false, false);
		//Resets the randomizers to a new seed value
		srand(time(0));
	}
	else
	{
		//If initialization fails, sets "isRunning" to false and instantly closes window
		isRunning = false;
	}
}

//The function that runs every frame to perfrom the tasks of the game
void Game::update()
{
	//Refreshes the manager and performs any updates the manager has to perform (mostly
	//just for entity deletion)
	manager.refresh();
	manager.update();
	//Performs the buttonControl function to check if any buttons have been pressed
	buttonControl();
	//Loads the board to keep it rendered in an up-to-date fashion
	board->LoadBoard(initial);
}

//Renders everything in the window every frame
void Game::render()
{
	//Deletes all graphics shown in the window
	SDL_RenderClear(renderer);
	board->DrawBoard();
	//Renders the entities at their current position
	manager.draw();
	//Shows the rendered graphics in the window
	SDL_RenderPresent(renderer);
}

//Handles the events in the game, technically only handles the X button being pressed on the window
void Game::events()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
		//If the X button is clicked...
	case SDL_QUIT:
		//...close the game
		isRunning = false;
		break;

	default:
		break;
	}
}

//Handles the closing of the game windown and the exiting of the program
void Game::clean()
{
	//Deletes window
	SDL_DestroyWindow(window);
	//Deletes the render for the window
	SDL_DestroyRenderer(renderer);
	//Quits the program
	SDL_Quit();
}
