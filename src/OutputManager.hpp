#ifndef CONNECTFOUR_PRINTMANAGER_H
#define CONNECTFOUR_PRINTMANAGER_H

#include <iostream>
#include <string>

#include "GameBoard.hpp"
#include "OutputColorizer.hpp"

namespace connect_four
{

class OutputManager
{
public:
	OutputManager(void);

	static inline void PrintUsage(void)
	{
		std::cout << std::endl
			<< "Usage:" << std::endl
			<< "connect_four <Options>" << std::endl
			<< std::endl
			<< "Options:" << std::endl
			<< "\tNone: Use program with in-game menus" << std::endl
			<< "\t--pvp: Play PlayerVsPlayer GameMode" << std::endl
			<< "\t--pvc: Play PlayerVsComputer GameMode" << std::endl
			<< "\t--cvp: Play ComputerVsPlayer GameMode" << std::endl
			<< "\t--pvp: Play ComputerVsComputer GameMode" << std::endl
			<< "\t--alphabeta: Disable alpha beta pruning" << std::endl
			<< "\t--difficulty <easy|normal|hard>: Specify AI difficulty"
			<< std::endl << std::endl;
	}

	/*
	 * Print given board in color and with a frame
	 */
	void PrintGameBoard(const GameBoard* gameBoard) const;

	inline void PrintMainMenu(void) const
	{
		std::cout << std::endl
			<< "Choose game mode: " << std::endl
			<< "[1]\tPlayer versus Player" << std::endl
			<< "[2]\tPlayer versus Computer" << std::endl
			<< "[3]\tComputer versus Computer" << std::endl
			<< std::endl
			<< "[0]\tExit Program" << std::endl;
	}

	inline void PrintHumanVsAiMenu(void) const
	{
		std::cout << std::endl
			<< "Choose who should start the game: " << std::endl
			<< "[1]\tPlayer" << std::endl
			<< "[2]\tComputer" << std::endl;
	}

	inline void PrintAiDifficultyMenu(void) const
	{
		std::cout << std::endl
			<< "Choose difficulty (Depth of NegaMax algorithm): " << std::endl
			<< "[1]\tEasy\t(3)" << std::endl
			<< "[2]\tNormal\t(5)" << std::endl
			<< "[3]\tHard\t(7)" << std::endl;
	}

	inline void PrintAlphaBetaMenu(void) const
	{
		std::cout << std::endl
			<< "Choose Alpha Beta Pruning Option: " << std::endl
			<< "[1]\tEnable Pruning" << std::endl
			<< "[2]\tDisable Pruning" << std::endl;
	}

	inline void PrintTurnIntro(const uint8_t currentPlayer) const
	{
		colorizer::SetOutputColorToPlayer(currentPlayer);
		std::cout << std::endl << "Player " << currentPlayer + 1;
		colorizer::ResetOutputColor();
		std::cout << ": " << std::endl;
	}

	inline void PrintWinner(const uint8_t currentPlayer) const
	{
		colorizer::SetOutputColorToPlayer(currentPlayer);
		std::cout << std::endl << "Player " << currentPlayer + 1;
		colorizer::ResetOutputColor();
		std::cout << " has won!" << std::endl;
	}

	inline void PrintRematchMenu(void) const
	{
		std::cout << "What do you want to do next?" << std::endl
			<< "[0]\tExit Program" << std::endl
			<< "[1]\tPlay new game" << std::endl;
	}

	inline void PrintDraw(void) const
	{
		std::cout << std::endl
			<< "Tied game! Better luck next time..." << std::endl
			<< std::endl
			<< "What do you want to do next?" << std::endl
			<< "[0]\tExit Program" << std::endl
			<< "[1]\tPlay new game" << std::endl;
	}

private:
	const size_t m_StringLength;
	std::string m_Border;
	std::string m_BoardBottom;
};

} // namespace connect_four

#endif // !CONNECTFOUR_PRINTMANAGER_H
