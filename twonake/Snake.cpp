#include "Snake.h"



Snake::Snake(int x, int y)
	:head(std::make_pair(x, y))
	,headDirection(0)
	,nextDirection(0)
	,length(4)
{
	// front | snakeTail <-<-<- snakeHead | back
	body.emplace_back(head.first, head.second + 3);
	body.emplace_back(head.first, head.second + 2);
	body.emplace_back(head.first, head.second + 1);
	body.push_back(head);
}

void Snake::turnHead()
{

	if (nextDirection == Straight) return;

	if (nextDirection == Left) headDirection--;
	if (nextDirection == Right) headDirection++;

	headDirection = (headDirection + 4) % 4;

	nextDirection = Straight;
}

std::pair<int, int> Snake::getBody(int bodyNum) const
{
	std::pair<int, int> piece = body.at(bodyNum);

	return piece;
}

int Snake::getLength() const
{
	return length;
}

int Snake::getDirection() const
{
	return headDirection;
}

void Snake::nextStep(bool isBite)
{
	std::pair<int, int> nextDirection = getDirectionPoint();

	head.first += nextDirection.first;
	head.second += nextDirection.second;

	// ‰Ê•¨‚ğH‚×‚é‚Æ‚«‚Í“ª‚ğL‚Î‚· = “ª‚Ì•ûŒü‚É1‚Â‚¾‚¯’Ç‰Á‚·‚ê‚Î—Ç‚¢B
	if (isBite)
	{
		body.push_back(head);
		length++;

		return;
	}

	for (int i = 0; i < length - 1; i++)
	{
		body[i] = body[i + 1];
	}

	body[length - 1] = head;
}

std::pair<int, int> Snake::getDirectionPoint() const
{
	if (headDirection == 0) return std::make_pair(0, -1);
	if (headDirection == 1) return std::make_pair(1, 0);
	if (headDirection == 2) return std::make_pair(0, 1);
	if (headDirection == 3) return std::make_pair(-1, 0);
}

void Snake::setNextDirection(int direction)
{
	nextDirection = direction;
}