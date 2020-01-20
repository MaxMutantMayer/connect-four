#include "OutputColorizer.hpp"

#include <windows.h>

#define RED   12
#define BLU   9
#define RESET 15

namespace connect_four
{
namespace colorizer
{

HANDLE g_consoleHandle;

void Init(void)
{
	g_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	ResetOutputColor();
}

void SetOutputColorToPlayer(uint8_t currentPlayer)
{
	currentPlayer == 0
		? SetConsoleTextAttribute(g_consoleHandle, RED)
		: SetConsoleTextAttribute(g_consoleHandle, BLU);
}

void ResetOutputColor(void)
{
	SetConsoleTextAttribute(g_consoleHandle, RESET);
}

} // namespace colorizer
} // namespace connect_four
