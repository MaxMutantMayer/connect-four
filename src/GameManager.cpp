#include "GameManager.hpp"

#include <iostream>

#include "ComputerPlayer.hpp"
#include "HumanPlayer.hpp"

namespace connect_four
{

GameManager::GameManager(const GameSettings& settings)
	: m_settings(settings)
	, m_Players{ nullptr, nullptr }
	, m_GameBoard(new GameBoard())
	, m_CurrentPlayer(0)
{}

GameManager::~GameManager(void)
{
	ClearPlayers();
	delete m_GameBoard;
}

void GameManager::StartGame(void)
{
	if (!m_settings.useMenus)
	{
		Run();
	}
	else
	{
		int input = -1;
		while (input != 0)
		{
			if ((input = MainMenu()) != 0)
			{
				// In HumanVsAI -> check wether human or ai should start
				if (input == 2)
				{
					input = HumanVsAiMenu();
				}
				m_settings.mode = static_cast<GameMode::Type>(input);

				if (m_settings.mode != GameMode::PlayerVsPlayer)
				{
					m_settings.difficulty = DifficultyMenu();
					m_settings.useAlphaBetaPruning = AlphaBetaMenu();
				}

				Run();

				if ((input = RematchMenu()) == 1)
				{
					//  Board and players need to be cleared before rematch
					Cleanup();
				}
			}
		}
	}
}

void GameManager::InitializePlayers(void)
{
	switch (m_settings.mode)
	{
	case GameMode::PlayerVsPlayer:
		m_Players[0] = new HumanPlayer();
		m_Players[1] = new HumanPlayer();
		break;
	case GameMode::PlayerVsComputer:
		m_Players[0] = new HumanPlayer();
		m_Players[1] = new ComputerPlayer(PlayerID::PlayerTwo,
				m_settings.difficulty, m_settings.useAlphaBetaPruning);
		break;
	case GameMode::ComputerVSComputer:
		m_Players[0] = new ComputerPlayer(PlayerID::PlayerOne,
				m_settings.difficulty, m_settings.useAlphaBetaPruning);
		m_Players[1] = new ComputerPlayer(PlayerID::PlayerTwo,
				m_settings.difficulty, m_settings.useAlphaBetaPruning);
		break;
	case GameMode::ComputerVsPlayer:
		m_Players[0] = new ComputerPlayer(PlayerID::PlayerOne,
				m_settings.difficulty, m_settings.useAlphaBetaPruning);
		m_Players[1] = new HumanPlayer();
		break;
	default:
		std::cerr << ("Unknown game mode!") << std::endl;
		break;
	}
}

void GameManager::Run(void)
{
	uint8_t playerSymbol = 0;
	int selectedColumn = 0;
	bool winner = false;

	ClearPlayers();
	InitializePlayers();
	m_OutputManager.PrintGameBoard(m_GameBoard);

	while (!winner)
	{
		playerSymbol = m_CurrentPlayer + 1;

		m_OutputManager.PrintTurnIntro(m_CurrentPlayer);
		selectedColumn = m_Players[m_CurrentPlayer]->DoTurn(m_GameBoard);
		m_GameBoard->AddDisc(selectedColumn, playerSymbol);

		m_OutputManager.PrintGameBoard(m_GameBoard);
		winner = m_GameBoard->CheckForWin();

		//Check for tied game
		if (!winner && m_GameBoard->IsFull())
		{
			m_OutputManager.PrintDraw();
			return;
		}

		SwapPlayers();
	}

	// Swap again for correct winner output
	SwapPlayers();
	m_OutputManager.PrintWinner(m_CurrentPlayer);
}

} // namespace connect_four
