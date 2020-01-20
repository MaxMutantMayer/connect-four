#ifndef CONNECTFOUR_POSITION_H
#define CONNECTFOUR_POSITION_H

namespace connect_four
{

struct Position
{
	Position(void)
		: row(0)
		, col(0)
	{}

	explicit Position(int x, int y)
		: row(x)
		, col(y)
	{}

	int row;
	int col;
};

} // namespace connect_four

#endif // !CONNECTFOUR_POSITION_H
