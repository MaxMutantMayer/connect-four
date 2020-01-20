#ifndef CONNECTFOUR_PLAYER_H
#define CONNECTFOUR_PLAYER_H

#include "GameBoard.hpp"

namespace connect_four
{

class Player
{
public:
	virtual ~Player(void){}
	virtual int DoTurn(GameBoard* inout_board) const = 0;
};

} // namespace connect_four

#endif // !CONNECTFOUR_PLAYER_H
