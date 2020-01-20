#include "ComputerPlayer.hpp"

#include <chrono>
#include <iostream>
#include <limits.h>

#include "mutantutility/StopWatch.hpp"

#include "Position.hpp"

namespace connect_four
{

/*
 * NegaMax constants
 */
const int ALPHA_START_VALUE = -40000;
const int BETA_START_VALUE = 40000;

ComputerPlayer::ComputerPlayer(PlayerID::Type id, int difficulty,
	bool usePruning)
	: m_ID(id)
	, m_difficulty(difficulty)
	, m_usePruning(usePruning)
{}

int ComputerPlayer::DoTurn(GameBoard* inout_board) const
{
	int nextMove = -1;
	uint8_t playerSymbol = static_cast<uint8_t>(m_ID);

	mutant_utility::StopWatch stopWatch;
	m_usePruning
		? NegaMaxAlphaBeta(inout_board, m_difficulty, playerSymbol,
			ALPHA_START_VALUE, BETA_START_VALUE, nextMove)
		: NegaMax(inout_board, m_difficulty, playerSymbol, nextMove);
	double decisionTimeMs = stopWatch.ToMilliseconds();

	std::cout << "It took me " << decisionTimeMs << " ms to choose column: "
		<< nextMove + 1 << std::endl;

	return nextMove;
}

int ComputerPlayer::NegaMax(GameBoard* inout_board, int depth,
	uint8_t playerSymbol, int& out_nextMove) const
{
	int best_score = INT_MIN;
	int temp_score = INT_MIN;
	int temp_nextMove = -1;
	Position lastMove(0, 0);

	if (ReachedEndOfSearch(inout_board, playerSymbol, depth, temp_score))
		return temp_score;

	for (int column = 0; column < GameDefines::COLS; ++column)
	{
		if (inout_board->IsValidMove(column))
		{
			// Save previous last move to reset board later
			lastMove = inout_board->m_lastMove;

			inout_board->AddDisc(column, playerSymbol);
			temp_score = -NegaMax(inout_board, depth - 1,
				(playerSymbol % 2) + 1, out_nextMove);
			inout_board->UndoLastMove(lastMove);

			if (temp_score > best_score)
			{
				best_score = temp_score;
				temp_nextMove = column;
			}
		}
	}

	out_nextMove = temp_nextMove;
	return best_score;
}

int ComputerPlayer::NegaMaxAlphaBeta(GameBoard* inout_board, int depth,
	uint8_t playerSymbol, int alpha, int beta, int& out_nextMove) const
{
	int best_score = INT_MIN;
	int temp_score = INT_MIN;
	int temp_nextMove = -1;
	Position lastMove(0, 0);

	if (ReachedEndOfSearch(inout_board, playerSymbol, depth, temp_score))
		return temp_score;

	for (int column = 0; column < GameDefines::COLS; ++column)
	{
		if (inout_board->IsValidMove(column))
		{
			// Save previous last move to reset board later
			lastMove = inout_board->m_lastMove;

			inout_board->AddDisc(column, playerSymbol);
			temp_score = -NegaMaxAlphaBeta(inout_board, depth - 1,
				(playerSymbol % 2) + 1, -beta, -alpha, out_nextMove);
			inout_board->UndoLastMove(lastMove);

			if (temp_score > best_score)
			{
				best_score = temp_score;
				temp_nextMove = column;
			}

			// Pruning
			alpha = temp_score >= alpha ? temp_score : alpha;
			if (alpha >= beta)
			{
				break;
			}
		}
	}

	out_nextMove = temp_nextMove;
	return best_score;
}

bool ComputerPlayer::ReachedEndOfSearch(const GameBoard* board,
	uint8_t playerSymbol, int currentDepth, int& out_score) const
{
	bool reachedEnd = false;

	if (currentDepth == 0)
	{
		out_score = Evaluate(board);
		reachedEnd = true;
	}
	else if (CheckIfGameEnd(board, out_score))
	{
		// Multiply score with depth to prefer faster wins
		out_score *= currentDepth;
		reachedEnd = true;
	}

	if (playerSymbol != static_cast<uint8_t>(m_ID))
		out_score *= -1;

	return reachedEnd;
}

bool ComputerPlayer::CheckIfGameEnd(const GameBoard* board,
	int& out_score) const
{
	if (board->CheckForWin())
	{
		// Check if it was my win
		out_score =
			board->GetDiscAt(board->m_lastMove) == static_cast<uint8_t>(m_ID)
			? 10000
			: -10000;

		return true;
	}
	else if (board->IsFull())
	{
		out_score = 0;
		return true;
	}

	return false;
}

/*
 * Evaluate current state of board:
 * .)	Check if game is finished
 *
 * .)	Check if AI or Opponent has 3 discs threat
 *		-> If player_1 -> prefer odd threats
 *		-> if player_2 -> prefer even threats
 *
 * .)	Check if AI or opponent has disc pair
 *
 * .)	Increase score for every positive threat and
 *		decrease score for every negative threat
 *		-> Bonus for odd rows if Ai == player 1
 *		   and bonus for even rows if Ai == player 2 (Zugzwang)
 *
 * Although number of discs to win can be changed in GameDefines
 * this function only works properly for the default value (4)
 */
int ComputerPlayer::Evaluate(const GameBoard* board) const
{
	int temp_score = 0;

	// Check if last move leaded to a win
	if (CheckIfGameEnd(board, temp_score))
	{
		return temp_score;
	}

	// Temporary save threats
	std::vector<Threat> positiveMajorThreats;
	std::vector<Threat> negativeMajorThreats;
	std::vector<Threat> positiveMinorThreats;
	std::vector<Threat> negativeMinorThreats;

	// Get all threats from current board
	GetAllThreats(board, positiveMajorThreats, negativeMajorThreats,
		positiveMinorThreats, negativeMinorThreats);

	const int EvenOrOdd = (m_ID == 1) ? 1 : 0;

	// First check negative major threats
	// If threat can be connected -> fixed win for opponent
	// Otherwise check if threat lies on a preferred accessible position
	for (Threat& t : negativeMajorThreats)
	{
		for (Position& p : t.positionsToComplete)
		{
			if (CanPositionBePlayed(board, p))
			{
				return -10000;
			}
			else if ((p.row % 2) != EvenOrOdd)
			{
				temp_score -= 100;
			}
			else
			{
				temp_score -= 60;
			}
		}
	}

	// Check for positive major threats and if there is a fixed win
	size_t countToWin = 0;
	for (Threat& t : positiveMajorThreats)
	{
		for (Position& p : t.positionsToComplete)
		{
			if (CanPositionBePlayed(board, p))
			{
				if (++countToWin >= 2)
				{
					return 10000;
				}
			}
			else if ((p.row % 2) == EvenOrOdd)
			{
				temp_score += 100;
			}
			else
			{
				temp_score += 60;
			}
		}
		countToWin = 0;
	}

	// Check for opponents minor threats and if there is a fixed win
	for (Threat& t : negativeMinorThreats)
	{
		for (Position& p : t.positionsToComplete)
		{
			if (CanPositionBePlayed(board, p))
			{
				if (++countToWin >= 3)
				{
					return -10000;
				}
			}
			else if ((p.row % 2) != EvenOrOdd)
			{
				temp_score -= 25;
			}
			else
			{
				temp_score -= 10;
			}
		}
		countToWin = 0;
	}

	// Check for positive minor threats
	for (Threat& t : positiveMinorThreats)
	{
		for (Position& p : t.positionsToComplete)
		{
			if ((p.row % 2) != EvenOrOdd)
			{
				temp_score += 25;
			}
			else
			{
				temp_score += 10;
			}
		}
	}

	return temp_score;
}

/*
 * In this huge functions all threats are gathered by iterating over the board
 * in all 4 directions (horizontal, vertical, left diagonal and right diagonal)
 */
void ComputerPlayer::GetAllThreats(const GameBoard* board,
	std::vector<Threat>& out_positiveMajorThreats,
	std::vector<Threat>& out_negativeMajorThreats,
	std::vector<Threat>& out_positiveMinorThreats,
	std::vector<Threat>& out_negativeMinorThreats) const
{
	const BoardArray& b = board->GetBoard();

	Threat tempThreat;
	uint8_t tempColor = 0;
	size_t discCounter = 0;
	size_t completionCounter = 0;

	// Temporary save space between connection of disc
	int emptyPosititionRow = 0, emptyPosititionColumn = 0;
	bool emptySpaceBetween = false;

	/*
	 * Iterate board horizontal (-)
	 */
	for (int current_row = 0; current_row < GameDefines::ROWS; ++current_row)
	{
		for (int current_column = 0; current_column < GameDefines::COLS;
			++current_column)
		{
			if (b[current_row][current_column] != 0)
			{
				tempColor = b[current_row][current_column];
				while (current_column < GameDefines::COLS)
				{
					if (b[current_row][current_column] == tempColor)
					{
						discCounter == 0
							? tempThreat.startPos =
								Position(current_row, current_column)
							: tempThreat.endPos =
								Position(current_row, current_column);

						++discCounter;
						++current_column;
						continue;
					}
					else if (b[current_row][current_column] == 0)
					{
						// Check if color continues after empty space
						// Stop if this is already the second space
						if (current_column + 1 < GameDefines::COLS &&
							b[current_row][current_column + 1] == tempColor &&
							!emptySpaceBetween)
						{
							emptyPosititionColumn = current_column;

							emptySpaceBetween = true;
							++completionCounter;
							tempThreat.positionsToComplete.push_back(
								Position(current_row, emptyPosititionColumn));

							++current_column;
							continue;
						}
					}
					// Other players color
					else if (b[current_row][current_column] != 0)
					{
						// Set back position
						// -> Restarts this loop for other color
						--current_column;
					}
					break;
				}
			}
			// Add as threat if there is space before or after discs
			if (discCounter >= 2)
			{
				// If there are 3 discs and a empty space in this threat
				// -> do not check for other positions to complete, since
				// this is the only position which will lead to a connection
				if (discCounter >= 3 && emptySpaceBetween)
				{
					tempColor == static_cast<uint8_t>(m_ID)
						? out_positiveMajorThreats.push_back(tempThreat)
						: out_negativeMajorThreats.push_back(tempThreat);
				}
				else
				{
					// Check for space before discs
					if (tempThreat.startPos.col - 1 >= 0 &&
						b[current_row][tempThreat.startPos.col - 1] == 0)
					{
						++completionCounter;
						tempThreat.positionsToComplete.push_back(
							Position(current_row, tempThreat.startPos.col - 1));
					}

					// Check for space after discs
					if (tempThreat.endPos.col + 1 < GameDefines::COLS &&
						b[current_row][tempThreat.endPos.col + 1] == 0)
					{
						++completionCounter;
						tempThreat.positionsToComplete.push_back(
							Position(current_row, tempThreat.endPos.col + 1));
					}

					if (discCounter >= 3 && completionCounter >= 1)
					{
						tempColor == static_cast<uint8_t>(m_ID)
							? out_positiveMajorThreats.push_back(tempThreat)
							: out_negativeMajorThreats.push_back(tempThreat);
					}
					else
					{
						// If there is a space within threat
						// -> do not take +2/-2 positions into account
						// because they won't complete the threat
						if (emptySpaceBetween)
						{
							// Check for space 2 cols before discs
							if (tempThreat.startPos.col - 2 >= 0 &&
								b[current_row][tempThreat.startPos.col - 2]
									== 0)
							{
								++completionCounter;
								tempThreat.positionsToComplete.push_back(
									Position(current_row,
										tempThreat.startPos.col - 2));
							}

							// Check for space 2 cols after discs
							if (tempThreat.endPos.col + 2 < GameDefines::COLS &&
								b[current_row][tempThreat.endPos.col + 2] == 0)
							{
								++completionCounter;
								tempThreat.positionsToComplete.push_back(
									Position(current_row,
										tempThreat.endPos.col + 2));
							}
						}

						if (completionCounter >= 2)
						{
							tempColor == static_cast<uint8_t>(m_ID)
								? out_positiveMinorThreats.push_back(
									tempThreat)
								: out_negativeMinorThreats.push_back(
									tempThreat);
						}
					}
				}
			}

			// Set current position back if space was taken into account
			if (emptySpaceBetween)
			{
				current_column = emptyPosititionColumn;
			}

			emptySpaceBetween = false;
			discCounter = 0;
			completionCounter = 0;
			tempThreat.positionsToComplete.clear();
		}
	}

	/*
	 * Iterate board vertical (|)
	 */
	for (int current_column = 0; current_column < GameDefines::COLS;
		++current_column)
	{
		int current_row = GameDefines::ROWS - 1;

		if (b[current_row][current_column] != 0)
		{
			tempColor = b[current_row][current_column];
			while (current_row >= 0 &&
				b[current_row][current_column] == tempColor)
			{
				discCounter == 0
					? tempThreat.startPos =
						Position(current_row, current_column)
					: tempThreat.endPos =
						Position(current_row, current_column);

				++discCounter;
				--current_row;
			}

			if (current_row >= 0)
			{
				if (b[current_row][current_column] == 0)
				{
					tempThreat.positionsToComplete.push_back(
						Position(current_row, current_column));

					if (discCounter >= 3)
					{
						tempColor == static_cast<uint8_t>(m_ID)
							? out_positiveMajorThreats.push_back(tempThreat)
							: out_negativeMajorThreats.push_back(tempThreat);
					}
					else
					{
						--current_row;
						if (current_row >= 0 && discCounter == 2 &&
							b[current_row][current_column] == 0)
						{
							tempThreat.positionsToComplete.push_back(
								Position(current_row, current_column));

							tempColor == static_cast<uint8_t>(m_ID)
								? out_positiveMinorThreats.push_back(
									tempThreat)
								: out_negativeMinorThreats.push_back(
									tempThreat);
						}
					}
				}
			}

			discCounter = 0;
			tempThreat.positionsToComplete.clear();
		}
	}

	const int numDiagonals = GameDefines::ROWS + GameDefines::COLS - 1;

	/*
	 * Iterate board right diagonal (/)
	 */
	for (int slice = 0; slice < numDiagonals; ++slice)
	{
		int row_start = slice >= GameDefines::ROWS
			? GameDefines::ROWS - 1
			: slice;

		int row_stop = slice < GameDefines::COLS
			? 0
			: slice - GameDefines::COLS + 1;

		for (int current_row = row_start; current_row >= row_stop;
			--current_row)
		{
			int current_column = slice - current_row;

			if (b[current_row][current_column] != 0)
			{
				tempColor = b[current_row][current_column];
				while (current_row >= 0 && current_column < GameDefines::COLS)
				{
					if (b[current_row][current_column] == tempColor)
					{
						discCounter == 0
							? tempThreat.startPos =
								Position(current_row, current_column)
							: tempThreat.endPos =
								Position(current_row, current_column);

						++discCounter;

						--current_row;
						++current_column;
						continue;
					}
					else if (b[current_row][current_column] == 0)
					{
						// Check if color continues after empty space
						// Stop if this is already the second space
						if (current_column - 1 >= 0 &&
							current_column + 1 < GameDefines::COLS &&
							b[current_row - 1][current_column + 1]
								== tempColor &&
							!emptySpaceBetween)
						{
							emptySpaceBetween = true;
							emptyPosititionRow = current_row;
							emptyPosititionColumn = current_column;

							++completionCounter;
							tempThreat.positionsToComplete.push_back(
								Position(emptyPosititionRow,
									emptyPosititionColumn));

							--current_row;
							++current_column;
							continue;
						}
					}
					// Other players color
					else if (b[current_row][current_column] != 0)
					{
						// Set back position
						// -> Restarts this loop for other color
						++current_row;
						--current_column;
					}
					break;
				}
			}

			// Add as threat if there is space before or after discs
			if (discCounter >= 2)
			{
				// If there are 3 discs and a empty space in this threat
				// -> do not check for other positions to complete, since
				// this is the only position which will lead to a connection
				if (discCounter >= 3 && emptySpaceBetween)
				{
					tempColor == static_cast<uint8_t>(m_ID)
						? out_positiveMajorThreats.push_back(tempThreat)
						: out_negativeMajorThreats.push_back(tempThreat);
				}
				else
				{
					// Check for space before discs
					if (tempThreat.startPos.row + 1 < GameDefines::ROWS &&
						tempThreat.startPos.col - 1 >= 0 &&
						b[tempThreat.startPos.row + 1][tempThreat.startPos.col - 1]
							== 0)
					{
						++completionCounter;
						tempThreat.positionsToComplete.push_back(
							Position(tempThreat.startPos.row + 1,
								tempThreat.startPos.col - 1));
					}

					// Check for space after discs
					if (tempThreat.endPos.row - 1 >= 0 &&
						tempThreat.endPos.col + 1 < GameDefines::COLS &&
						b[tempThreat.endPos.row - 1][tempThreat.endPos.col + 1]
							== 0)
					{
						++completionCounter;
						tempThreat.positionsToComplete.push_back(
							Position(tempThreat.endPos.row - 1,
								tempThreat.endPos.col + 1));
					}

					if (discCounter >= 3 && completionCounter >= 1)
					{
						tempColor == static_cast<uint8_t>(m_ID)
							? out_positiveMajorThreats.push_back(tempThreat)
							: out_negativeMajorThreats.push_back(tempThreat);
					}
					else
					{
						// If there is a space within threat
						// -> do not take +2/-2 positions into account
						// because they won't complete threat
						if (emptySpaceBetween)
						{
							// Check for space 2 cols before discs
							if (tempThreat.startPos.row + 2 < GameDefines::ROWS &&
								tempThreat.startPos.col - 2 >= 0 &&
								b[tempThreat.startPos.row + 2][tempThreat.startPos.col - 2]
									== 0)
							{
								++completionCounter;
								tempThreat.positionsToComplete.push_back(
									Position(tempThreat.startPos.row + 2,
										tempThreat.startPos.col - 2));
							}

							// Check for space 2 cols after discs
							if (tempThreat.endPos.row - 2 >= 0 &&
								tempThreat.endPos.col + 2 < GameDefines::COLS &&
								b[tempThreat.endPos.row - 2][tempThreat.endPos.col + 2]
									== 0)
							{
								++completionCounter;
								tempThreat.positionsToComplete.push_back(
									Position(tempThreat.endPos.row - 2,
										tempThreat.endPos.col + 2));
							}
						}

						if (completionCounter >= 2)
						{
							tempColor == static_cast<uint8_t>(m_ID)
								? out_positiveMinorThreats.push_back(
									tempThreat)
								: out_negativeMinorThreats.push_back(
									tempThreat);
						}
					}
				}
			}

			// Set current position back if space was taken into account
			if (emptySpaceBetween)
			{
				current_row = emptyPosititionRow;
				current_column = emptyPosititionColumn;
			}

			emptySpaceBetween = false;
			discCounter = 0;
			completionCounter = 0;
			tempThreat.positionsToComplete.clear();
		}
	}

	/*
	 * Iterate board left diagonal (\)
	 */
	for (int slice = 0; slice < numDiagonals; ++slice)
	{
		int row_start = slice < GameDefines::COLS
			? GameDefines::ROWS - 1
			: GameDefines::ROWS - (slice - (GameDefines::ROWS - 1));

		int row_stop = slice < GameDefines::ROWS
			? GameDefines::ROWS - 1 - slice
			: 0;

		for (int current_row = row_start; current_row >= row_stop;
			--current_row)
		{
			int current_column = slice - ((GameDefines::ROWS - 1)
				- current_row);

			if (b[current_row][current_column] != 0)
			{
				tempColor = b[current_row][current_column];
				while (current_row >= 0 && current_column >= 0)
				{
					if (b[current_row][current_column] == tempColor)
					{
						discCounter == 0
							? tempThreat.startPos =
								Position(current_row, current_column)
							: tempThreat.endPos =
								Position(current_row, current_column);

						++discCounter;

						--current_row;
						--current_column;
						continue;
					}
					else if (b[current_row][current_column] == 0)
					{
						// Check if color continues after empty space
						// Stop if this is already the second space
						if (current_column - 1 >= 0 &&
							current_column - 1 >= 0 &&
							b[current_row - 1][current_column - 1]
								== tempColor &&
							!emptySpaceBetween)
						{
							emptySpaceBetween = true;
							emptyPosititionRow = current_row;
							emptyPosititionColumn = current_column;

							++completionCounter;
							tempThreat.positionsToComplete.push_back(
								Position(emptyPosititionRow,
									emptyPosititionColumn));

							--current_row;
							--current_column;
							continue;
						}
					}
					// Other players color
					else if (b[current_row][current_column] != 0)
					{
						// Set back position
						// -> Restarts this loop for other color
						++current_row;
						++current_column;
					}
					break;
				}
			}

			// Add as threat if there is space before or after discs
			if (discCounter >= 2)
			{
				// If there are 3 discs and a empty space in this threat
				// -> do not check for other positions to complete, since
				// this is the only position which will lead to a connection
				if (discCounter >= 3 && emptySpaceBetween)
				{
					tempColor == static_cast<uint8_t>(m_ID)
						? out_positiveMajorThreats.push_back(tempThreat)
						: out_negativeMajorThreats.push_back(tempThreat);
				}
				else
				{
					// Check for space before discs
					if (tempThreat.startPos.row + 1 < GameDefines::ROWS &&
						tempThreat.startPos.col + 1 < GameDefines::COLS &&
						b[tempThreat.startPos.row + 1][tempThreat.startPos.col + 1]
							== 0)
					{
						++completionCounter;
						tempThreat.positionsToComplete.push_back(
							Position(tempThreat.startPos.row + 1,
								tempThreat.startPos.col + 1));
					}

					// Check for space after discs
					if (tempThreat.endPos.row - 1 >= 0 &&
						tempThreat.endPos.col - 1 >= 0 &&
						b[tempThreat.endPos.row - 1][tempThreat.endPos.col - 1]
							== 0)
					{
						++completionCounter;
						tempThreat.positionsToComplete.push_back(
							Position(tempThreat.endPos.row - 1,
								tempThreat.endPos.col - 1));
					}

					if (discCounter >= 3 && completionCounter >= 1)
					{
						tempColor == static_cast<uint8_t>(m_ID)
							? out_positiveMajorThreats.push_back(tempThreat)
							: out_negativeMajorThreats.push_back(tempThreat);
					}
					else
					{
						// If there is a space within threat
						// -> do not take +2/-2 positions into account
						// because they won't complete threat
						if (emptySpaceBetween)
						{
							// Check for space 2 cols before discs
							if (tempThreat.startPos.row + 2 < GameDefines::ROWS &&
								tempThreat.startPos.col + 2 < GameDefines::COLS &&
								b[tempThreat.startPos.row + 2][tempThreat.startPos.col + 2]
									== 0)
							{
								++completionCounter;
								tempThreat.positionsToComplete.push_back(
									Position(tempThreat.startPos.row + 2,
										tempThreat.startPos.col + 2));
							}

							// Check for space 2 cols after discs
							if (tempThreat.endPos.row - 2 >= 0 &&
								tempThreat.endPos.col - 2 >= 0 &&
								b[tempThreat.endPos.row - 2][tempThreat.endPos.col - 2]
									== 0)
							{
								++completionCounter;
								tempThreat.positionsToComplete.push_back(
									Position(tempThreat.endPos.row - 2,
										tempThreat.endPos.col - 2));
							}
						}

						if (completionCounter >= 2)
						{
							tempColor == static_cast<uint8_t>(m_ID)
								? out_positiveMinorThreats.push_back(
									tempThreat)
								: out_negativeMinorThreats.push_back(
									tempThreat);
						}
					}
				}
			}
			// Set current position back if space was taken into account
			if (emptySpaceBetween)
			{
				current_row = emptyPosititionRow;
				current_column = emptyPosititionColumn;
			}

			emptySpaceBetween = false;
			discCounter = 0;
			completionCounter = 0;
			tempThreat.positionsToComplete.clear();
		}
	}
}

bool ComputerPlayer::CanPositionBePlayed(const GameBoard* board,
		const Position& position) const
{
	const BoardArray& b = board->GetBoard();

	if (position.row + 1 < GameDefines::ROWS
		&& b[position.row + 1][position.col] != 0)
	{
		return true;
	}

	return false;
}

} // namespace connect_four
