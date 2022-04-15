#ifndef KYLINOTHELLOAI_HPP
#define KYLINOTHELLOAI_HPP

#include "OthelloAI.hpp"
#include "OthelloGameState.hpp"
#include "OthelloBoard.hpp"
#include <vector>

namespace jiahag3
{
	class KylinOthelloAI : public OthelloAI
	{
	public:
        std::vector<std::pair<int,int>> allValidMoves(const OthelloGameState& state);	//loop over the board to find all valid move
		virtual std::pair<int, int> chooseMove(const OthelloGameState& state);		//After get the best move using search, return it
		int search(OthelloGameState& s, int alpha, int beta, int depth, OthelloCell& mycolor);		//use Alpha-beta pruning to search for the best move
		int evaluation(OthelloGameState& state, OthelloCell myColor);	//loop over the board to check the greater value to return
	};

}


#endif