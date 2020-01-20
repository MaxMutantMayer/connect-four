#ifndef CONNECTFOUR_GAMEMANAGER_H
#define CONNECTFOUR_GAMEMANAGER_H

#include "GameBoard.hpp"
#include "GameSettings.hpp"
#include "InputController.hpp"
#include "OutputManager.hpp"
#include "Player.hpp"

namespace connect_four
{

class GameManager
{
public:
	explicit GameManager(const GameSettings& settings);
	~GameManager(void);

	/*
	 * Either directly enter game loop if input arguments were provided,
	 * or query game settings via menus and then enter loop
	 */
	void StartGame(void);

private:
	void InitializePlayers(void);

	/*
	 * Game main loop
	 */
	void Run(void);

	/*
	 * Query game mode
	 * \return Chosen game mode
		0 - Exit
		1 - Player Vs Player
		2 - Player Vs Computer
		3 - Computer Vs Computer
	 */
	inline int MainMenu(void) const
	{
		m_OutputManager.PrintMainMenu();
		return InputControl::GetIntegerInput(0, 3);
	}

	/*
	 * Query whether AI should start first
	 * \return GameMode value - 2 (Player first) or 4 (AI first)
	 */
	inline int HumanVsAiMenu(void) const
	{
		m_OutputManager.PrintHumanVsAiMenu();
		return InputControl::GetIntegerInput(1, 2) * 2;
	}

	/*
	 * Query AI difficulty (depth)
	 * \return Depth of NegaMax algorithm
	 */
	inline int DifficultyMenu(void) const
	{
		// Difficulty input is mapped from [1,2,3] to depth [3,5,7]
		m_OutputManager.PrintAiDifficultyMenu();
		return InputControl::GetIntegerInput(1, 3) * 2 + 1;
	}

	/*
	 * Query if alpha beta pruning should be enabled
	 * \return True if pruning is enabled
	 */
	inline bool AlphaBetaMenu(void) const
	{
		m_OutputManager.PrintAlphaBetaMenu();
		return InputControl::GetIntegerInput(1, 2) == 1;
	}

	/*
	 * Query whether a rematch is wished
	 * \return True if rematch
	 */
	inline int RematchMenu(void) const
	{
		m_OutputManager.PrintRematchMenu();
		return InputControl::GetIntegerInput(0, 1);
	}

	inline void SwapPlayers(void)
	{
		m_CurrentPlayer = !m_CurrentPlayer;
	}

	inline void ClearPlayers(void)
	{
		delete m_Players[0];
		delete m_Players[1];
	}

	inline void Cleanup(void)
	{
		m_GameBoard->ClearBoard();
		m_CurrentPlayer = 0;
	}

	OutputManager m_OutputManager;
	GameSettings m_settings;
	Player* m_Players[2];
	GameBoard* m_GameBoard;
	uint8_t m_CurrentPlayer;
};

} // namespace connect_four

#endif // !CONNECTFOUR_GAMEMANAGER_H
