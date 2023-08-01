#pragma once

// Base square
struct Square
{
	int x;
	int y;
	int size;
	int color;
	int row;
	int column;
	bool visitedThisTurn;

	Square(int newRow, int newColumn, int newSize, int newColor) :
		size(newSize), x(newColumn* size), y(newRow* size), color(newColor), row(newRow), column(newColumn)
	{
		x = newColumn * size;
		y = newRow * size;
		visitedThisTurn = false;
	}
};