#include "InputController.hpp"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "OutputManager.hpp"

namespace connect_four
{

bool InputControl::ParseInputArguments(int argc, char* argv[],
	GameSettings& out_settings)
{
	if (argc == 1)
	{
		out_settings.useMenus = true;
		return true;
	}

	std::vector<std::string> args(argv + 1, argv + argc);
	std::string inputParameter = "";
	bool validUsage = false;

	// Nothing fancy here; just query each argument
	for (size_t i = 0; i < args.size(); ++i)
	{
		inputParameter = args[i];
		if (inputParameter.compare("--pvp") == 0)
		{
			out_settings.mode = GameMode::PlayerVsPlayer;
			validUsage = true;
		}
		else if (inputParameter.compare("--pvc") == 0)
		{
			out_settings.mode = GameMode::PlayerVsComputer;
			validUsage = true;
		}
		else if (inputParameter.compare("--cvp") == 0)
		{
			out_settings.mode = GameMode::ComputerVsPlayer;
			validUsage = true;
		}
		else if (inputParameter.compare("--cvc") == 0)
		{
			out_settings.mode = GameMode::ComputerVSComputer;
			validUsage = true;
		}
		else if (inputParameter.compare("--alphabeta") == 0)
		{
			out_settings.useAlphaBetaPruning = false;
		}
		else if (inputParameter.compare("--difficulty") == 0)
		{
			if (i + 1 < args.size())
			{
				++i;
				if (args[i].compare("easy") == 0)
				{
					out_settings.difficulty = 3;
				}
				else if (args[i].compare("normal") == 0)
				{
					out_settings.difficulty = GameDefines::DEFAULT_DIFFICULTY;
				}
				else if (args[i].compare("hard") == 0)
				{
					out_settings.difficulty = 7;
				}
				else
				{
					std::cerr << "Value of difficulty must be either \"easy\", "
						<< "\"normal\" or \"hard\"!" << std::endl;
					OutputManager::PrintUsage();

					return false;
				}
			}
			else
			{
				std::cerr << "Value of difficulty is missing!" << std::endl;
				OutputManager::PrintUsage();

				return false;
			}
		}
		else
		{
			std::cerr << "Unknown input parameter: " << inputParameter
				<< std::endl;
			OutputManager::PrintUsage();

			return false;
		}
	}

	if (argc >= 2 && !validUsage)
	{
		std::cerr << "Wrong input parameters!" << std::endl;
		OutputManager::PrintUsage();

		return false;
	}

	return true;
}

int InputControl::GetIntegerInput(int minimum, int maximum)
{
	int input = 0;

	while (1)
	{
		while (!(std::cin >> input))
		{
			// Clear and ignore wrong input
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(),'\n');
			std::cerr << "Invalid input. Please try again: " << std::endl;
		}

		if (input >= minimum && input <= maximum)
		{
			break;
		}
		else
		{
			std::cerr << "Input must be in range [" << minimum << ", "
				<< maximum << "]. Please try again: " << std::endl;
		}
	}

	return input;
}

} // namespace connect_four
