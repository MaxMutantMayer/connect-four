#ifndef CONNECTFOUR_COMPUTERPLAYER_H
#define CONNECTFOUR_COMPUTERPLAYER_H

#include <vector>

#include "Player.hpp"
#include "Position.hpp"

namespace connect_four
{

struct PlayerID
{
	enum Type
	{
		PlayerOne = 1,
		PlayerTwo = 2
	};
};

/*
 * Threats are incomplete series of discs, that can lead to a win.
 *
 * They can be divided into:
 * Positiv: Series that leads to my win
 * Negativ: Series that leads to opponents win
 * Major: Series of 3 discs (1 disc needed to win)
 * Minor: Series of 2 discs (2 discs needed to win)
 */
struct Threat
{
	Position startPos;
	Position endPos;

	// Positions which would lead to a win
	std::vector<Position> positionsToComplete;
};

class ComputerPlayer : public Player
{
public:
	explicit ComputerPlayer(PlayerID::Type id, int difficulty,
			bool usePruning);

	virtual int DoTurn(GameBoard* inout_board) const override;

private:
	int NegaMax(GameBoard* inout_board, int depth, uint8_t playerSymbol,
			int& out_nextMove) const;

	/*
	 * Same as 'NegaMax' with pruning
	 */
	int NegaMaxAlphaBeta(GameBoard* inout_board, int depth,
		uint8_t playerSymbol, int alpha, int beta, int& out_nextMove) const;

	/*
	 * Check if base case of NegaMax is reached
	 * \return True if maximum depth is reached or game ended
	 */
	bool ReachedEndOfSearch(const GameBoard* board, uint8_t playerSymbol,
		int currentDepth, int& out_score) const;

	bool CheckIfGameEnd(const GameBoard* board, int& out_score) const;

	/*
	 * Heuristic function
	 * Creates score of given board by evaluating all threats
	 */
	int Evaluate(const GameBoard* board) const;

	/*
	 * All threats (positive minor & major + negative minor & major)
	 * are gathered and written to the given 'out_'-vectors
	 */
	void GetAllThreats(const GameBoard* board,
			std::vector<Threat>& out_positiveMajorThreats,
			std::vector<Threat>& out_negativeMajorThreats,
			std::vector<Threat>& out_positiveMinorThreats,
			std::vector<Threat>& out_negativeMinorThreats) const;

	bool CanPositionBePlayed(const GameBoard* board,
		const Position& position) const;

	PlayerID::Type m_ID;
	const int m_difficulty;
	const bool m_usePruning;
};

} // namespace connect_four

#endif // !CONNECTFOUR_COMPUTERPLAYER_H
