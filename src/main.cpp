/*
 * Connect Four
 *
 * Author:	Maximilian Mayer
 * License:	MIT (see LICENSE file)
 */

#include "GameManager.hpp"
#include "GameSettings.hpp"
#include "InputController.hpp"

int main(int argc, char* argv[])
{
	// Handle input arguments
	connect_four::GameSettings settings;
	if (!connect_four::InputControl::ParseInputArguments(argc, argv, settings))
		return 1;

	// Start game with specified settings
	connect_four::GameManager gm(settings);
	gm.StartGame();

	return 0;
}
