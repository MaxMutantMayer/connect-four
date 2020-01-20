#include "OutputManager.hpp"

namespace connect_four
{

OutputManager::OutputManager(void)
	: m_StringLength(GameDefines::COLS * 2 - 1)
	, m_Border(m_StringLength, '-')
	, m_BoardBottom(m_StringLength, ' ')
{
	m_Border.insert(0, "   ");
	m_Border.append("   ");
	m_BoardBottom.insert(0, " | ");
	m_BoardBottom.append(" | ");

	colorizer::Init();
}

void OutputManager::PrintGameBoard(const GameBoard* gameBoard) const
{
	const BoardArray& board = gameBoard->GetBoard();

	// Header with column numbers
	std::cout << std::endl << "   ";
	for (size_t i = 1; i <= GameDefines::COLS; ++i)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl << m_Border << std::endl;

	// Field with discs
	uint8_t slotValue = 0;
	for (size_t i = 0; i < GameDefines::ROWS; ++i)
	{
		std::cout << " | ";
		for (size_t j = 0; j < GameDefines::COLS; ++j)
		{
			slotValue = board[i][j];
			if (slotValue == 0)
			{
				std::cout << "0 ";
			}
			else
			{
				colorizer::SetOutputColorToPlayer(slotValue - 1);
				std::cout << "0 ";
				colorizer::ResetOutputColor();
			}
		}
		std::cout << "| " << std::endl;
	}

	// Footer
	std::cout << m_Border << std::endl << m_BoardBottom << std::endl;
}

} // namespace connect_four
