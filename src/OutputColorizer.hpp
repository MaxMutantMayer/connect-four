#ifndef CONNECTFOUR_OUTPUTCOLORIZER_H
#define CONNECTFOUR_OUTPUTCOLORIZER_H

#include <stdint.h>

namespace connect_four
{
namespace colorizer
{

void Init(void);
void SetOutputColorToPlayer(uint8_t currentPlayer);
void ResetOutputColor(void);

} // namespace colorizer
} // namespace connect_four

#endif // !CONNECTFOUR_OUTPUTCOLORIZER_H
