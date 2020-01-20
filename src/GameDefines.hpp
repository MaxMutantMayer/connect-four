#ifndef CONNECTFOUR_GAMEDEFINES_H
#define CONNECTFOUR_GAMEDEFINES_H

namespace connect_four
{
namespace GameDefines
{

/*
 * Although ROWS, COLS and CONNECT_TO_WIN can be modified here,
 * the AI's heuristic function only works properly with these settings.
 * Alter only for PvP mode.
 */
const int ROWS = 6;
const int COLS = 7;
const int CONNECT_TO_WIN = 4;

const int DEFAULT_DIFFICULTY = 5;

} // namespace GameDefines
} // namespace connect_four

#endif // !CONNECTFOUR_GAMEDEFINES_H
