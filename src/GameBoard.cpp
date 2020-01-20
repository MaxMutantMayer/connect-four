#include "GameBoard.hpp"

namespace connect_four
{

GameBoard::GameBoard(void)
	: m_lastMove(-1, -1)
{
	memset(m_gameBoard, 0, sizeof(BoardArray));
}

void GameBoard::AddDisc(int column, uint8_t playerSymbol)
{
	for (int i = (GameDefines::ROWS - 1); i >= 0; --i)
	{
		if (m_gameBoard[i][column] == 0)
		{
			m_gameBoard[i][column] = playerSymbol;

			// Save position of inserted disc
			m_lastMove = Position(i, column);

			break;
		}
	}
}

bool GameBoard::CheckForWin(void) const
{
	if (m_lastMove.row == -1 || m_lastMove.col == -1)
		return false;

	// Check by using last inserted disc
	uint8_t playerSymbol =
		static_cast<uint8_t>(m_gameBoard[m_lastMove.row][m_lastMove.col]);

	if (CountHorizontal(playerSymbol) >= GameDefines::CONNECT_TO_WIN ||
		CountVertical(playerSymbol) >= GameDefines::CONNECT_TO_WIN ||
		CountLeftDiagonal(playerSymbol) >= GameDefines::CONNECT_TO_WIN ||
		CountRightDiagonal(playerSymbol) >= GameDefines::CONNECT_TO_WIN)
		return true;

	return false;
}


bool GameBoard::IsFull(void) const
{
	for (size_t col = 0; col < GameDefines::COLS; ++col)
	{
		if (m_gameBoard[0][col] == 0)
			return false;
	}
	return true;
}

size_t GameBoard::CountHorizontal(uint8_t playerSymbol) const
{
	int col;
	size_t counter = 1;

	// Left
	for (col = m_lastMove.col - 1; col >= 0 &&
			m_gameBoard[m_lastMove.row][col] == playerSymbol; --col, ++counter);

	// Right
	for (col = m_lastMove.col + 1; col < GameDefines::COLS &&
			m_gameBoard[m_lastMove.row][col] == playerSymbol; ++col, ++counter);

	return counter;
}

size_t GameBoard::CountVertical(uint8_t playerSymbol) const
{
	int row;
	size_t counter = 1;

	// Downwards
	for (row = m_lastMove.row + 1; row < GameDefines::ROWS &&
			m_gameBoard[row][m_lastMove.col] == playerSymbol; ++row, ++counter);

	// Upwards
	for (row = m_lastMove.row - 1; row >= 0 &&
			m_gameBoard[row][m_lastMove.col] == playerSymbol; --row, ++counter);

	return counter;
}

size_t GameBoard::CountLeftDiagonal(uint8_t playerSymbol) const
{
	int row, col;
	size_t counter = 1;

	// Downwards
	for (row = m_lastMove.row + 1, col = m_lastMove.col + 1;
			row < GameDefines::ROWS && col < GameDefines::COLS &&
			m_gameBoard[row][col] == playerSymbol; ++row, ++col, ++counter);

	// Upwards
	for (row = m_lastMove.row - 1, col = m_lastMove.col - 1;
			row >= 0 && col >= 0 && m_gameBoard[row][col] == playerSymbol;
			--row, --col, ++counter);

	return counter;
}

size_t GameBoard::CountRightDiagonal(uint8_t playerSymbol) const
{
	int row, col;
	size_t counter = 1;

	// Downwards
	for (row = m_lastMove.row + 1, col = m_lastMove.col - 1;
			row < GameDefines::ROWS && col >= 0 &&
			m_gameBoard[row][col] == playerSymbol; ++row, --col, ++counter);

	// Upwards
	for (row = m_lastMove.row - 1, col = m_lastMove.col + 1;
			row >= 0 && col < GameDefines::COLS &&
			m_gameBoard[row][col] == playerSymbol; --row, ++col, ++counter);

	return counter;
}

} // namespace connect_four
