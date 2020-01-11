#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <random>

using namespace std;

#define WHITE 7
#define GREEN 2
#define YELLOW 14
#define MAGENTA 13
#define BLUE 9
#define RED 4

void ClearScreen()
{
	int n;
	for (n = 0; n < 10; n++)
		printf("\n\n\n"); //"\n\n\n\n\n\n\n\n\n\n" );
}

class FloodIt
{ // working game that output data
private:
	vector<vector<int>> board;
	vector<vector<int>> spread; //vector for checking the spread of color
	//int score          //maybe
	int i = 0, j = 0; //used for movement within vector
	int chosenColor;
	int boardWidth = 20, boardHeight = 6;
	int turnCnt = 999;
	bool exitFlag = false;
	bool resetFlag = false;
	string whitespace =
	{ "                           " };
	HANDLE hConsole;
	char keyPressed;
	char keyName[6] = { 'q', 'w', 'e', 'a', 's', 'd' };
	int colors[6] = { GREEN, RED, BLUE, YELLOW, WHITE, MAGENTA };
	bool win = false;
	bool easyMode = false;
	default_random_engine generator;
	std::discrete_distribution<int> distribution{ 40, 2, 3, 4, 10, 40, 10, 4, 3, 2, 40 };

public:
	FloodIt(); //dimensions of board, limit of moves,

	void gameLoop(); //inf loop used to controll the game
	void gameInit();
	void tileSpread(int color, int row, int column);
	//void colorSpread(int color);
	void drawBoard(vector<vector<int>>& board);
	void setSize(int height, int width);
	void genBoard();
	int randTile();
	void gameLogic();
	void kbPress();
	void checkWin();
	void endRound();
	void gameReset();
	int cinInt();
	void zeroCheck(int& val);
};

FloodIt::FloodIt()
{
	gameInit();
	gameLoop();
}

//Main game loop. Runs infinitely as long as the player doesnt exit.
void FloodIt::gameLoop()
{
	while (1)
	{
		Sleep(100);
		ClearScreen();
		checkWin();
		drawBoard(board);
		endRound();
		if (resetFlag) gameReset();
		//drawBoard(spread); //just for testing
		kbPress();
		gameLogic();
		if (exitFlag)
			break;

	}
};

//Checking if all the tiles are in one color by using spread vector.
void FloodIt::checkWin()
{
	bool breakLoop = false;
	for (int k = 0; k < (boardHeight); k++)
	{
		for (int l = 0; l < (boardWidth); l++)
		{
			if (!spread[k][l])
			{
				breakLoop = true;
				break;
			}
			if (l == (boardWidth - 1) && k == (boardHeight - 1))
				win = true;
		}
		if (breakLoop)
		{
			break;
		}
	}
}

//Ending procedure that shows the win or lose screen and resets the game.
void FloodIt::endRound()
{
	if (win || turnCnt <= 0)
	{
		if (win)
		{
			cout << "You won. Press any key to reset the game." << endl;
		}
		else if (turnCnt <= 0)
		{
			cout << "You lose. Press any key to reset the game." << endl;
		}
		_getch();
		FlushConsoleInputBuffer(hConsole);
		gameReset();
	}
}

//Reset procedure.
void FloodIt::gameReset()
{

	board.clear();
	spread.clear();
	resetFlag = false;
	int tmpHeight, tmpWidth;
	cout << "Insert size of the board: " << endl
		<< "Height: ";
	zeroCheck(tmpHeight);
	cout << "Width: ";
	zeroCheck(tmpWidth);
	cout << "Easy mode? 1- yes, 0 - no" << endl;
	easyMode = cinInt();
	setSize(tmpHeight, tmpWidth);
	genBoard(); //board generation
	chosenColor = board[0][0];
	win = false;
	turnCnt = 999;
	drawBoard(board);
}

//Getting the value from keyboard for the color, reset or exit.
void FloodIt::kbPress()
{
	keyPressed = _getch();
	switch (keyPressed) //converting the keys to int color values
	{
	case 'q':
		chosenColor = 2;
		break;
	case 'w':
		chosenColor = 4;
		break;
	case 'e':
		chosenColor = 9;
		break;
	case 'a':
		chosenColor = 14;
		break;
	case 's':
		chosenColor = 7;
		break;
	case 'd':
		chosenColor = 13;
		break;
	case 'r':
		resetFlag = true;
		break;
	case (char)27: //ESC key
		exitFlag = true;
		break;
	default:
		cout << "Wrong key pressed" << endl;
		break;
	}
}

//Mechanics for spreading 
void FloodIt::gameLogic()
{
	//colorSpread(chosenColor);
	for (int k = 0; k < board.size(); k++)
	{
		for (int l = 0; l < board[0].size(); l++)
		{
			if (spread[k][l] == true)
				tileSpread(chosenColor, k, l);
		}
	}
	//colorSpread(chosenColor);
}

//Random generation of the colors of the tiles on the board.
void FloodIt::genBoard()
{
	board.resize(boardHeight, vector<int>(boardWidth, 0));
	spread.resize(boardHeight, vector<int>(boardWidth, 0));

	for (int k = 0; k < (boardHeight); k++)
	{
		for (int l = 0; l < (boardWidth); l++)
		{
			if (easyMode)
				board[k][l] = colors[abs((int)distribution(generator)) % 6];
			else
				board[k][l] = colors[rand() % 6];
			spread[k][l] = 0;
		}
	}
	tileSpread(board[0][0], 0, 0); //checking if the first tile has any neighbouring tiles in the same color
}

//Setting up all needed parameters and the board.
void FloodIt::gameInit()
{
	int tmpHeight, tmpWidth;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //cmd reference
	srand(time(NULL));                          //for rand generation of tiles
	cout << "Insert size of the board: " << endl
		<< "Height: ";
	//cin >> tmpHeight;
	zeroCheck(tmpHeight);
	cout << "Width: ";
	//cin >> tmpWidth;
	zeroCheck(tmpWidth);
	cout << "Easy mode? 1- yes, 0 - no" << endl;
	//cin >> easyMode; 
	easyMode = cinInt();
	setSize(tmpHeight, tmpWidth);
	genBoard(); //board generation
	chosenColor = board[0][0];

	//tileSpread(board[0][0], 0, 0);                        //checking if the first tile has any neighbouring tiles in the same color
	/*board.resize(boardHeight, vector<int>(boardWidth, 4));
	for(int k = 0; k < (boardHeight);k++){
	   for(int l = 0; l < (boardWidth);l++){
		  cout<<board[k][l];
	   }
	   cout<<endl;
	}*/
}

//DEPRECATED. Literally doesnt do anything useful. Not needed at all. Absolute waste. Bad.
/*
void FloodIt::colorSpread(int color){
   for(int k = 0 ; k < board.size(); k++){
	  for(int l = 0 ; l < board[0].size(); l++){
		 if(spread[k][l] == true){
			board[k][l] = color;
			//spread[k][l] = false;
		 }
	  }
   }
}*/

//Function checks if adjacent tiles are in the same color of the tile and marks them as spread and changes color to the chosen one. The spread is used to see the progress of spreading the color.
void FloodIt::tileSpread(int color, int row, int column)
{
	int temp;
	spread[row][column] = true;
	board[row][column] = color;
	if (!((column + 1) >= boardWidth) && (board[row][column + 1] == color) && (spread[row][column + 1] == false))
	{
		/*cout<<"Right"<<endl;
		cout<<"row "<<row<<endl;
		cout<<"col "<<column<<endl;*/
		temp = column + 1;
		tileSpread(color, row, temp);
	}
	if (!((row + 1) >= boardHeight) && (board[row + 1][column] == color) && (spread[row + 1][column] == false))
	{
		/*cout<<"Down"<<endl;
		cout<<"row "<<row<<endl;
		cout<<"col "<<column<<endl;*/
		temp = row + 1;
		cout << temp << endl;
		tileSpread(color, temp, column);
	}
	if (!((column - 1) < 0) && (board[row][column - 1] == color) && (spread[row][column - 1] == false))
	{
		/*cout<<"Left"<<endl;
		cout<<"row "<<row<<endl;
		cout<<"col "<<column<<endl;*/
		temp = column - 1;
		tileSpread(color, row, temp);
	}
	if (!((row - 1) < 0) && (board[row - 1][column] == color) && (spread[row - 1][column] == false))
	{
		/*cout<<"Up"<<endl;
		cout<<"row "<<row<<endl;
		cout<<"col "<<column<<endl;*/
		temp = row - 1;
		tileSpread(color, temp, column);
	}
}

//Drawing the board on the screen as well as other elements
void FloodIt::drawBoard(vector<vector<int>>& board)
{
	for (int k = 0; k < (boardHeight + 2); k++)
	{
		cout << whitespace;
		for (int l = 0; l < (boardWidth + 2); l++)
		{
			if (k == 0 && l == 0)
			{
				cout << (char)201;
			}
			else if ((k == (boardHeight + 1) && l == 0))
			{
				cout << (char)200;
			}
			else if (k == 0 && l == (boardWidth + 1))
			{
				cout << (char)187;
			}
			else if (k == (boardHeight + 1) && l == (boardWidth + 1))
			{
				cout << (char)188;
			}
			else if (k == 0 || k == (boardHeight + 1))
			{
				cout << (char)205;
			}
			else if (l == 0 || l == (boardWidth + 1))
			{
				cout << (char)186;
			}
			else
			{
				SetConsoleTextAttribute(hConsole, board[k - 1][l - 1]);
				cout << (char)219;
				//cout<<board[k-1][l-1];
				SetConsoleTextAttribute(hConsole, WHITE);
			}
		}
		cout << endl;
	}

	for (int k = 0; k < (sizeof(keyName) / sizeof(keyName[0])); k++)
	{
		cout << keyName[k];
		SetConsoleTextAttribute(hConsole, colors[k]);
		cout << (char)219 << " ";
		SetConsoleTextAttribute(hConsole, WHITE);
	}
	cout << " ESC - end of program" << "   r - new game";
	cout << endl;
}

//Setting size for the board.
void FloodIt::setSize(int height, int width)
{
	boardWidth = width;
	boardHeight = height;
}

//Inputting from cin and checking if the input is an integer.
int FloodIt::cinInt()
{
	bool isInt = false;
	int temp;
	string in;
	while (1)
	{
		cin >> in;

		for (int k = 0; k < in.length(); k++)
		{
			if (isdigit(in[k]) == false)
			{
				isInt = false;
			}
			else isInt = true;

		}
		if (isInt)
		{
			break;
		}
		else
			cout << "Error: Not an integer. Try again" << endl;
	}

	return (stoi(in));
}

//Checking if the integer value is not 0.
void FloodIt::zeroCheck(int& val) {
	while (1) {
		val = cinInt();
		if (val != 0) break;
		else cout << "Value cannot be zero. Try again" << endl;
	}
}

//game checking for same color tiles chekcks from top left
//need a recursive function that checks all directions of the tile if its in the same color
//

int main()
{
	FloodIt game;
	/*int b = 219;          //could be useful later
	 cin >> b;
	 char a = (char)b;
	 cout << a;*/
	 /*vector<vector<int>> board = {{1, 2, 3}, {8, 6, 4}};
	 cout << board.size() << endl;
	 cout << board[0].size() << endl;*/
	return 0;
}