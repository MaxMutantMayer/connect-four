#ifndef CONNECTFOUR_GAMEMODES_H
#define CONNECTFOUR_GAMEMODES_H

namespace connect_four
{

struct GameMode
{
	enum Type
	{
		None = 0,
		PlayerVsPlayer = 1,
		PlayerVsComputer = 2,
		ComputerVSComputer = 3,
		ComputerVsPlayer = 4
	};
};

} // namespace connect_four

#endif // !CONNECTFOUR_GAMEMODES_H
