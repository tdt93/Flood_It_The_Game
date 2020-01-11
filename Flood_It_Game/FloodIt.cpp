#include "FLOODIT.H"
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <random>

using namespace std;

//Constructor to settup and play game:
FloodIt::FloodIt()
{
	gameInit();
	gameLoop();
}

//Setting up all needed parameters and the board.
void FloodIt::gameInit()
{

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //cmd reference
	srand(time(NULL));                          //for rand generation of tiles
	chooseMode();
	genBoard();									//board generation
	chosenColor = board[0][0];

	
}

void FloodIt::chooseMode() {
	cout << "Welcome to Flood It" << endl << endl;
	while (1) {
		cout << "Please choose difficulty mode: " << endl;
		cout << "[1]Easy		[2]Normal	[3]Hard		[4]Custom" << endl;
		int mode = 0;
		mode = cinInt();
		if (mode == 1) {
			boardWidth = defDim;
			boardHeight = defDim;
			maxTurn = 10;
			break;
		}
		else if (mode == 2) {
			boardWidth = defDim * 2;
			boardHeight = defDim * 2;
			maxTurn = 20;
			break;
		}
		else if (mode == 3) {
			boardWidth = defDim * 4;
			boardHeight = defDim * 4;
			maxTurn = 40;
			break;
		}
		else  if (mode == 4) {
			int tmpHeight, tmpWidth, tmpNum;
			cout << "Insert size of the board: " << endl
				<< "Height: ";
			zeroCheck(tmpHeight);
			cout << "Width: ";
			zeroCheck(tmpWidth);
			cout << "Easy mode? 1 - yes, 0 - no" << endl;
			oneOrZero(tmpNum);
			setSize(tmpHeight, tmpWidth);
			maxTurn = tmpHeight + tmpWidth;
			break;
		}
		else {
			cout << "Error: Invalid Input!!!" << endl;
			cout << "Please enter again: " << endl;
			mode = 0;
		}
		system("cls");
	}
}

//Inputting from cin and checking if the input is an integer.
int FloodIt::cinInt()
{
	bool isInt = false;
	string in;
	while (1)
	{
		cin >> in;

		for (unsigned int k = 0; k < in.length(); k++)
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

//Make sure the integer value is only 1 or 0:
void FloodIt::oneOrZero(int& num) {
	while (1) {
		num = cinInt();
		if (num == 0) {
			easyMode = false;
			break;
		}
		else if (num == 1) {
			easyMode = true;
			break;
		}
		else cout << "Invalid!!, Only enter 1 or 0. Try again: " << endl;
	}
}

//Setting size for the board.
void FloodIt::setSize(int height, int width)
{
	boardWidth = width;
	boardHeight = height;
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

//Main game loop. Runs infinitely as long as the player doesnt exit.
void FloodIt::gameLoop()
{
	while (1)
	{
		Sleep(100);
		system("cls");
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
				SetConsoleTextAttribute(hConsole, WHITE);
			}
		}
		cout << endl;
	}
	cout << endl;

	insBox();

	cout << "Turns: " << turnCnt << "/" << maxTurn << endl;
	if (wrongKey) {
		cout << "Wrong key pressed" << endl;
		wrongKey = false;
	}
}

//	Print out instruction box of colors to choose from:
void FloodIt::insBox() {
	cout << endl;
	cout << "------------ Color Box -----------" << endl;
	SetConsoleTextAttribute(hConsole, GREEN);
	cout << "Q: " << (char)219 << " GREEN  ";
	SetConsoleTextAttribute(hConsole, RED);
	cout << "W: " << (char)219 << " RED   ";
	SetConsoleTextAttribute(hConsole, BLUE);
	cout << "E: " << (char)219 << " BLUE " << endl;
	SetConsoleTextAttribute(hConsole, YELLOW);
	cout << "A: " << (char)219 << " YELLOW ";
	SetConsoleTextAttribute(hConsole, WHITE);
	cout << "S: " << (char)219 << " WHITE ";
	SetConsoleTextAttribute(hConsole, MAGENTA);
	cout << "D: " << (char)219 << " MAGENTA  " << endl;
	SetConsoleTextAttribute(hConsole, WHITE);
	cout << "     ESC: exit   R: restart" << endl;
	cout << "-----------------------------------" << endl;
}

//Ending procedure that shows the win or lose screen and resets the game.
void FloodIt::endRound()
{
	if (win || turnCnt >= maxTurn)
	{
		if (win)
		{
			cout << "You won. Press any key to reset the game." << endl;
		}
		else if (turnCnt >= maxTurn)
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
	system("cls");
	board.clear();
	spread.clear();
	resetFlag = false;
	chooseMode();
	genBoard(); //board generation
	chosenColor = board[0][0];
	win = false;
	turnCnt = 0;
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
		turnCnt++;
		break;
	case 'w':
		chosenColor = 4;
		turnCnt++;
		break;
	case 'e':
		chosenColor = 9;
		turnCnt++;
		break;
	case 'a':
		chosenColor = 14;
		turnCnt++;
		break;
	case 's':
		chosenColor = 7;
		turnCnt++;
		break;
	case 'd':
		chosenColor = 13;
		turnCnt++;
		break;
	case 'r':
		resetFlag = true;
		break;
	case (char)27: //ESC key
		exitFlag = true;
		break;
	default:
		wrongKey = true;
		break;
	}
}

//Mechanics for spreading 
void FloodIt::gameLogic()
{
	for (unsigned int k = 0; k < board.size(); k++)
	{
		for (unsigned int l = 0; l < board[0].size(); l++)
		{
			if (spread[k][l] == true)
				tileSpread(chosenColor, k, l);
		}
	}
}

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
		//cout << temp << endl;
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