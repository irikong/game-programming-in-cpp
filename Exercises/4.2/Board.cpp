// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Board.h"
#include "Random.h"

BoardState::BoardState()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			mBoard[i][j] = Empty;
		}
	}
}

std::vector<BoardState*> BoardState::GetPossibleMoves(SquareState player) const
{
	std::vector<BoardState*> retVal;

	// For each column, find if a move is possible
	for (int col = 0; col < 7; col++)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (mBoard[row][col] == BoardState::Empty)
			{
				retVal.emplace_back(new BoardState(*this));
				retVal.back()->mBoard[row][col] = player;
				break;
			}
		}
	}

	return retVal;
}

bool BoardState::IsTerminal() const
{
	// Is the board full?
	if (IsFull())
	{
		return true;
	}

	// Is there a four-in-a-row?
	int fourInRow = GetFourInARow();
	if (fourInRow != 0)
	{
		return true;
	}

	return false;
}

float BoardState::GetScore() const
{
	// If the board is full, the score is 0
	if (IsFull())
	{
		return 0.0f;
	}

	return CalculateHeuristic();
}

bool BoardState::IsFull() const
{
	bool isFull = true;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (mBoard[i][j] == Empty)
			{
				isFull = false;
			}
		}
	}

	return isFull;
}

int BoardState::GetFourInARow() const
{
	// Returns -1 if yellow wins, 1 if red wins, 0 otherwise

	// Check if there's a row with four in a row
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (mBoard[row][col] == mBoard[row][col + 1] &&
				mBoard[row][col] == mBoard[row][col + 2] &&
				mBoard[row][col] == mBoard[row][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a column with four in a row
	for (int col = 0; col < 7; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (mBoard[row][col] == mBoard[row + 1][col] &&
				mBoard[row][col] == mBoard[row + 2][col] &&
				mBoard[row][col] == mBoard[row + 3][col])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a right diagonal four in a row
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (mBoard[row][col] == mBoard[row + 1][col + 1] &&
				mBoard[row][col] == mBoard[row + 2][col + 2] &&
				mBoard[row][col] == mBoard[row + 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a left diagonal for in a row
	for (int col = 0; col < 4; col++)
	{
		for (int row = 3; row < 6; row++)
		{
			if (mBoard[row][col] == mBoard[row - 1][col + 1] &&
				mBoard[row][col] == mBoard[row - 2][col + 2] &&
				mBoard[row][col] == mBoard[row - 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

float BoardState::CalculateHeuristic() const
{
	float retVal = 0.0f;

	// Horizontal & Vertical
	for (int r = 0; r < 6; r++) {
		int c = 0, cnt = 0, temp = 0;
		while (c < 6) {
			if (mBoard[r][c] == mBoard[r][c + 1]) {
				cnt++;
			}
			else {
				temp = pow(cnt, 4);
				if (mBoard[r][c] == BoardState::Red) retVal += temp;
				else if (mBoard[r][c] == BoardState::Yellow) retVal -= temp;

				cnt = temp = 0;
			}

			c++;
		}

		if (cnt) {
			temp = pow(cnt, 4);
			if (mBoard[r][c] == BoardState::Red) retVal += temp;
			else if (mBoard[r][c] == BoardState::Yellow) retVal -= temp;
		}
	}
	for (int c = 0; c < 7; c++) {
		int r = 0, cnt = 0, temp = 0;
		while (r < 5) {
			if (mBoard[r][c] == mBoard[r + 1][c]) {
				cnt++;
			}
			else {
				temp = pow(cnt, 4);
				if (mBoard[r][c] == BoardState::Red) retVal += temp;
				else if (mBoard[r][c] == BoardState::Yellow) retVal -= temp;

				cnt = temp = 0;
			}

			r++;
		}

		if (cnt) {
			temp = pow(cnt, 4);
			if (mBoard[r][c] == BoardState::Red) retVal += temp;
			else if (mBoard[r][c] == BoardState::Yellow) retVal -= temp;
		}
	}

	// Diagonal
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (mBoard[row][col] == mBoard[row + 1][col + 1] &&
				mBoard[row][col] == mBoard[row + 2][col + 2] &&
				mBoard[row][col] == mBoard[row + 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Red)
				{
					retVal += 1000;
				}
				if (mBoard[row][col] == BoardState::Yellow)
				{
					retVal -= 1000;
				}
			}
		}
	}
	for (int col = 0; col < 4; col++)
	{
		for (int row = 3; row < 6; row++)
		{
			if (mBoard[row][col] == mBoard[row - 1][col + 1] &&
				mBoard[row][col] == mBoard[row - 2][col + 2] &&
				mBoard[row][col] == mBoard[row - 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Red)
				{
					retVal += 1000;
				}
				if (mBoard[row][col] == BoardState::Yellow)
				{
					retVal -= 1000;
				}
			}
		}
	}

	return retVal;
}

bool TryPlayerMove(BoardState* state, int column)
{
	// Find the first row in that column that's available
	// (if any)
	for (int row = 5; row >= 0; row--)
	{
		if (state->mBoard[row][column] == BoardState::Empty)
		{
			state->mBoard[row][column] = BoardState::Yellow;
			return true;
		}
	}

	return false;
}

void CPUMove(BoardState* state)
{
	*state = *AlphaBetaDecide(state, 4);
}

const BoardState* AlphaBetaDecide(const BoardState* root, int maxDepth)
{
	const BoardState* choice = nullptr;
	float maxValue = -std::numeric_limits<float>::infinity();
	float beta = std::numeric_limits<float>::infinity();
	for (const BoardState* child : root->GetPossibleMoves(BoardState::Red)) {
		float v = ABMin(child, maxDepth - 1, maxValue, beta);
		if (v > maxValue) {
			maxValue = v;
			choice = child;
		}
	}

	return choice;
}

float ABMax(const BoardState* node, int depth, float alpha, float beta)
{
	if (!depth || node->IsTerminal()) {
		return node->GetScore();
	}

	float maxValue = -std::numeric_limits<float>::infinity();
	for (const BoardState* child : node->GetPossibleMoves(BoardState::Red)) {
		maxValue = std::max(maxValue, ABMin(child, depth - 1, alpha, beta));
		if (maxValue >= beta) {
			return maxValue;
		}
		alpha = std::max(maxValue, alpha);
	}

	return maxValue;
}

float ABMin(const BoardState* node, int depth, float alpha, float beta)
{
	if (!depth || node->IsTerminal()) {
		return node->GetScore();
	}

	float minValue = std::numeric_limits<float>::infinity();
	for (const BoardState* child : node->GetPossibleMoves(BoardState::Yellow)) {
		minValue = std::min(minValue, ABMax(child, depth - 1, alpha, beta));
		if (minValue <= alpha) {
			return minValue;
		}
		beta = std::min(minValue, alpha);
	}

	return minValue;
}