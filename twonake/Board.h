#pragma once

#include"Snake.h"
#include<Siv3D.hpp>

enum FieldState
{
	None,
	Body,
	GreenHead,
	GreenItem,
	RedHead,
	RedItem,
	Route
};

class Board
{
	using pint = std::pair<int, int>;

private:
	Snake greenSnake;
	Snake redSnake;
	std::vector<std::vector<int>> field;
	void updateField(bool,bool);
	pint greenItem;
	pint redItem;
	bool fieldOut(pint) const;

	int greenScore;
	int redScore;

public:
	Board();
	Board(int, int);

	pint getSnakePiece(int, bool) const;
	int getSnakeLen(bool) const;
	int getSnakeDirection(bool) const;
	bool isSnakeDie() const;
	bool isGreenDie(pint, pint) const;
	bool isRedDie(pint, pint) const;
	bool nextStep();
	bool isGreenBite() const;
	bool isRedBite() const;
	void setNextDirection(int,int);
	pint getGreenItem(void) const;
	pint getRedItem(void) const;
	int getGreenScore(void) const;
	int getRedScore(void) const;

	const int xSize, ySize;
	
};
