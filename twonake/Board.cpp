#include "Board.h"

Board::Board()
	: xSize{ 10 }
	, ySize{ 10 }
	, greenSnake{ 10 / 2 - 3,10 / 2 }
	, redSnake{ 10 / 2 + 3,10 / 2 }
{

}

Board::Board(int x, int y)
	: xSize{ x }
	, ySize{ y }
	, greenSnake{ x / 2-3, y / 2 }
	, redSnake{ x / 2 + 3, y / 2 }
	, field{ std::vector<std::vector<int>>(x, std::vector<int>(y,0)) }
	, greenItem{ std::make_pair(3,3) }
	, redItem{ std::make_pair(x - 3,3) }
	, greenScore{ 0 }
	, redScore{ 0 }
{

}

using pint = std::pair<int, int>;

pint Board::getSnakePiece(int bodyNum, bool isGreen) const
{
	if (isGreen) return greenSnake.getBody(bodyNum);
	else return redSnake.getBody(bodyNum);
}

int Board::getSnakeLen(bool isGreen) const
{
	if (isGreen) return greenSnake.getLength();
	else return redSnake.getLength();
}

int Board::getSnakeDirection(bool isGreen) const
{
	if (isGreen) return greenSnake.getDirection();
	else return redSnake.getDirection();
}

bool Board::fieldOut(pint head) const
{
	if (head.first < 0 || xSize <= head.first) return true;
	if (head.second < 0 || ySize <= head.second) return true;

	return false;
}

bool Board::isSnakeDie() const
{
	pint greenHead = greenSnake.getBody(greenSnake.getLength() - 1);
	pint greenAdj = greenSnake.getDirectionPoint();
	pint redHead = redSnake.getBody(redSnake.getLength() - 1);
	pint redAdj = redSnake.getDirectionPoint();

	bool isDie = isGreenDie(greenHead, greenAdj) || isRedDie(redHead, redAdj);

	return isDie;
}

bool Board::isGreenDie(pint head, pint adj) const
{
	if (fieldOut(head)) return true;

	if (field[head.first][head.second] == Body) return true;
	if (field[head.first][head.second] == RedHead) return true;
	if (field[head.first][head.second] == RedItem) return true;

	return false;
}

bool Board::isRedDie(pint head, pint adj) const
{
	if (fieldOut(head)) return true;

	if (field[head.first][head.second] == Body) return true;
	if (field[head.first][head.second] == GreenHead) return true;
	if (field[head.first][head.second] == GreenItem) return true;

	return false;
}

bool Board::isGreenBite() const
{
	pint head = greenSnake.getBody(greenSnake.getLength() - 1);
	pint adj = greenSnake.getDirectionPoint();

	head.first += adj.first;
	head.second += adj.second;

	if (head.first < 0 || xSize <= head.first) return false;
	if (head.second < 0 ||ySize <= head.second) return false;

	if (field[head.first][head.second] == GreenItem) 
	{	
		return true; 
	}

	return false;
}

bool Board::isRedBite() const
{
	pint head = redSnake.getBody(redSnake.getLength() - 1);
	pint adj = redSnake.getDirectionPoint();

	head.first += adj.first;
	head.second += adj.second;

	if (head.first < 0 || xSize <= head.first) return false;
	if (head.second < 0 || ySize <= head.second) return false;

	if (field[head.first][head.second] == RedItem) return true;

	return false;
}

bool Board::nextStep()
{	
	greenSnake.turnHead();
	redSnake.turnHead();

	if (isSnakeDie()) return true;

	bool isGBite = isGreenBite();
	bool isRBite = isRedBite();

	if (isGBite)
	{
		greenScore++;
	}
	if (isRBite)
	{
		redScore++;
	}

	greenSnake.nextStep(isGBite);
	redSnake.nextStep(isRBite);

	if (fieldOut(greenSnake.getBody(greenSnake.getLength() - 1))) return true;
	if (fieldOut(redSnake.getBody(redSnake.getLength() - 1))) return true;

	updateField(isGBite, isRBite);

	return false;
}

void Board::setNextDirection(int greenDirection,int redDirection)
{
	greenSnake.setNextDirection(greenDirection);
	redSnake.setNextDirection(redDirection);
}

void Board::updateField(bool isGBite, bool isRBite)
{
	for (int x = 0; x < xSize; x++)
	{
		for (int y = 0; y < ySize; y++)
		{
			field[x][y] = None;
		}
	}

	{
		pint head = greenSnake.getBody(greenSnake.getLength() - 1);
		field[head.first][head.second] = GreenHead;

		head = redSnake.getBody(redSnake.getLength() - 1);
		field[head.first][head.second] = RedHead;
	}

	for (int i = 0; i < greenSnake.getLength()-1; i++)
	{
		pint piece = greenSnake.getBody(i);

		field[piece.first][piece.second] = Body;
	}

	{
		pint head = greenSnake.getBody(greenSnake.getLength()-1);
		pint adj = greenSnake.getDirectionPoint();

		while (0<= head.first && head.first < xSize && 0 <= head.second && head.second < ySize)
		{
			if (field[head.first][head.second] == None)
			{
				field[head.first][head.second] = Route;
			}

			head.first += adj.first;
			head.second += adj.second;
		}
	}

	for (int i = 0; i < redSnake.getLength()-1; i++)
	{
		pint piece = redSnake.getBody(i);

		field[piece.first][piece.second] = Body;
	}
	{
		pint head = redSnake.getBody(redSnake.getLength() - 1);
		pint adj = redSnake.getDirectionPoint();

		while (0 <= head.first && head.first < xSize && 0 <= head.second && head.second < ySize)
		{
			if (field[head.first][head.second] == None)
			{
				field[head.first][head.second] = Route;
			}

			head.first += adj.first;
			head.second += adj.second;
		}
	}

	{
		if (!isGBite)
		{
			field[greenItem.first][greenItem.second] = GreenItem;
		}
		if (!isRBite)
		{
			field[redItem.first][redItem.second] = RedItem;
		}
	}

	Array<pint> usablePoint;

	for (int x = 0; x < xSize; x++)
	{
		for (int y = 0; y < ySize; y++)
		{
			if (field[x][y] == None)
			{
				usablePoint.push_back(std::make_pair(x, y));
			}
		}
	}

	usablePoint.shuffle();

	if (isGBite)
	{
		greenItem = usablePoint[0];
		
		field[greenItem.first][greenItem.second] = GreenItem;

		usablePoint.pop_front();
	}

	if (isRBite)
	{
		redItem = usablePoint[0];

		field[redItem.first][redItem.second] = RedItem;
	}
}

pint Board::getGreenItem() const
{
	return greenItem;
}

pint Board::getRedItem() const
{
	return redItem;
}

int Board::getGreenScore() const
{
	return greenScore;
}

int Board::getRedScore() const
{
	return redScore;
}