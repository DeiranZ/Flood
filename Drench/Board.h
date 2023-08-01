#pragma once
#include <vector>
#include "Square.h"
#include "Vector2.h"

class Board
{
public:
	int boardSize;
	int squareSize;
	std::vector<Square> squares;

	Board(int newBoardSize, int newSquareSize) :
		boardSize(newBoardSize), squareSize(newSquareSize)
	{
		squares.reserve(newBoardSize * newBoardSize);
	}

	void AddSquare(Square square)
	{
		squares.emplace_back(square);
	}

	void AddSquare(int i, int j, int squareSize, int randomColorIndex)
	{
		squares.emplace_back(i, j, squareSize, randomColorIndex);
	}

	void GetSurroundingSquares(int x, int y, std::vector<Square*>* target)
	{
		Vector2 positions[4] = { Vector2(x - 1, y),
			Vector2(x, y - 1),
			Vector2(x, y + 1),
			Vector2(x + 1, y)
		};

		for (Vector2 position : positions)
		{
			bool isInsideBoard = (position.x >= 0 && position.x < this->boardSize) && (position.y >= 0 && position.y < this->boardSize);
			if (isInsideBoard)
			{
				target->push_back(&this->squares[position.x + position.y * this->boardSize]);
			}
		}
	}


	Board() {

	}
};