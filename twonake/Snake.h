#pragma once

#include<vector>

enum Direction
{
	Straight,
	Left,
	Right
};

class Snake
{
private:
	std::pair<int,int> head;
	int headDirection;
	int nextDirection;
	int length;
	std::vector<std::pair<int, int>> body;

public:
	Snake(int, int);
	void turnHead();
	std::pair<int,int> getBody(int) const;
	int getLength() const;
	int getDirection() const;
	void nextStep(bool);
	void setNextDirection(int);
	std::pair<int, int> getDirectionPoint() const;
};

