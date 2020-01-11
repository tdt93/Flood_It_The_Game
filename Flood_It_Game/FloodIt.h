// #pragma once
#ifndef FLOODIT_H
#define FLOODIT_H

#include <vector>
#include <string>
#include <windows.h>
#include <random>

#define WHITE 7
#define GREEN 2
#define YELLOW 14
#define MAGENTA 13
#define BLUE 9
#define RED 4

class FloodIt
{ 
private:
	std::vector<std::vector<int>> board;
	std::vector<std::vector<int>> spread;	//vector for checking the spread of color
	int chosenColor;
	int boardWidth, boardHeight;
	int turnCnt = 0;
	int defDim = 5;
	int maxTurn = 999;
	int colors[6] = { GREEN, RED, BLUE, YELLOW, WHITE, MAGENTA };
	bool exitFlag = false;
	bool resetFlag = false;
	bool modeChosen = false;
	std::string whitespace =
	{ "                           " };
	HANDLE hConsole;
	char keyPressed;
	bool win = false;
	bool easyMode = false;
	bool wrongKey = false;
	std::default_random_engine generator;
	std::discrete_distribution<int> distribution{ 40, 2, 3, 4, 10, 40, 10, 4, 3, 2, 40 };

public:
	FloodIt();							//dimensions of board, limit of moves,

	void gameInit();
	void chooseMode();
	int cinInt();
	void zeroCheck(int& val);
	void oneOrZero(int& num);
	void setSize(int height, int width);
	void genBoard();
	void gameLoop();
	void checkWin();
	void drawBoard(std::vector<std::vector<int>>& board);
	void insBox();
	void endRound();
	void gameReset();
	void kbPress();
	void gameLogic();
	void tileSpread(int color, int row, int column);
};

#endif // !FLOODIT_H
