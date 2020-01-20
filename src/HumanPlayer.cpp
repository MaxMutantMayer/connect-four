#include "HumanPlayer.hpp"

#include <iostream>

#include "InputController.hpp"

namespace connect_four
{

int HumanPlayer::DoTurn(GameBoard* inout_board) const
{
	int input = 0;
	bool validTurn = false;

	std::cout << "Select Column: ";
	while (!validTurn)
	{
		input = InputControl::GetIntegerInput(1, GameDefines::COLS);

		// Change input to actual board columns
		input -= 1;

		if ((validTurn = inout_board->IsValidMove(input)) == false)
		{
			std::cerr << std::endl <<
				"This column can't be used. Please try again: ";
		}
	}

	return input;
}


} // namespace connect_four
