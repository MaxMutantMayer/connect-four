#ifndef CONNECTFOUR_GAMESETTINGS_H
#define CONNECTFOUR_GAMESETTINGS_H

#include "GameDefines.hpp"
#include "GameModes.hpp"

namespace connect_four
{

struct GameSettings
{
	GameSettings(void)
		: mode(GameMode::None)
		, difficulty(GameDefines::DEFAULT_DIFFICULTY)
		, useAlphaBetaPruning(true)
		, useMenus(false)
	{}

	GameMode::Type mode;
	int difficulty;
	bool useAlphaBetaPruning;
	bool useMenus;
};

} // namespace connect_four

#endif // !CONNECTFOUR_GAMESETTINGS_H
