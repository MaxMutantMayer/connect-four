#ifndef CONNECTFOUR_INPUTCONTROLLER_H
#define CONNECTFOUR_INPUTCONTROLLER_H

#include "GameSettings.hpp"

namespace connect_four
{
namespace InputControl
{

/*
 * Parses arguments and sets them in out_settings
 * \return True if combination of arguments is valid
 */
bool ParseInputArguments(int argc, char* argv[], GameSettings& out_settings);

/*
 * Gathers user input until a valid value between [minimum, maximum] is entered
 * \return Valid entered integer
 */
int GetIntegerInput(int minimum, int maximum);

} // namespace InputControl
} // namespace connect_four

#endif // !CONNECTFOUR_INPUTCONTROLLER_H
