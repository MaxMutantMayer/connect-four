#ifndef CONNECTFOUR_HUMANPLAYER_H
#define CONNECTFOUR_HUMANPLAYER_H

#include "Player.hpp"

namespace connect_four
{

class HumanPlayer : public Player
{
public:
	virtual int DoTurn(GameBoard* inout_board) const override;
};

} // namespace connect_four

#endif // !CONNECTFOUR_HUMANPLAYER_H
