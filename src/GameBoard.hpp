#ifndef CONNECTFOUR_GAMEBOARD_H
#define CONNECTFOUR_GAMEBOARD_H

#include <stdint.h>
#include <string.h>

#include "GameDefines.hpp"
#include "Position.hpp"

namespace connect_four
{

typedef uint8_t BoardArray[GameDefines::ROWS][GameDefines::COLS];

class GameBoard
{
public:
	GameBoard(void);

	void AddDisc(int column, uint8_t playerSymbol);
	bool CheckForWin(void) const;
	bool IsFull(void) const;

	inline const BoardArray& GetBoard(void) const
	{
		return m_gameBoard;
	}

	inline uint8_t GetDiscAt(const Position& position) const
	{
		return m_gameBoard[position.row][position.col];
	}

	inline bool IsValidMove(int column) const
	{
		// Check only topmost row
		if (m_gameBoard[0][column] == 0)
			return true;

		return false;
	}

	/*
	 * Remove last inserted disc and specify 'previousPosition' as last move
	 */
	inline void UndoLastMove(const Position& previousPosition)
	{
		m_gameBoard[m_lastMove.row][m_lastMove.col] = 0;
		m_lastMove = Position(previousPosition.row, previousPosition.col);
	}

	inline void ClearBoard(void)
	{
		memset(m_gameBoard, 0, sizeof(BoardArray));
		m_lastMove = Position(-1, -1);
	}

	Position m_lastMove;

private:
	/*
	 * Count series, starting from last insert
	 */
	size_t CountHorizontal(uint8_t playerSymbol) const;     // (-)
	size_t CountVertical(uint8_t playerSymbol) const;       // (|)
	size_t CountLeftDiagonal(uint8_t playerSymbol) const;   // (\)
	size_t CountRightDiagonal(uint8_t playerSymbol) const;  // (/)

	BoardArray m_gameBoard;
};

} // namespace connect_four

#endif // !CONNECTFOUR_GAMEBOARD_H
