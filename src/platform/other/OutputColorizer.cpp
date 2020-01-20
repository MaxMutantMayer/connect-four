#include "OutputColorizer.hpp"

#include <stdio.h>

#define RED   "\x1B[31m"
#define BLU   "\x1B[34m"
#define RESET "\x1B[0m"

namespace connect_four
{
namespace colorizer
{

void Init(void)
{
	ResetOutputColor();
}

void SetOutputColorToPlayer(uint8_t currentPlayer)
{
	currentPlayer == 0
		? printf(RED)
		: printf(BLU);
}

void ResetOutputColor(void)
{
	printf(RESET);
}

} // namespace colorizer
} // namespace connect_four
