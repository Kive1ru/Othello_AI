#include "KylinOthelloAI.hpp"
#include "OthelloCell.hpp"
#include <ics46/factory/DynamicFactory.hpp>

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, jiahag3::KylinOthelloAI, "Kylin's OthelloAI (Required)");

std::vector<std::pair<int,int>> jiahag3::KylinOthelloAI::allValidMoves(const OthelloGameState& state){
	std::vector<std::pair<int,int>> allValid;
	for(int x = 0; x < state.board().width(); x++){
		for(int y = 0; y < state.board().height(); y++){
			if(state.isValidMove(x, y)){
				allValid.push_back(std::pair<int,int> (x,y));	//add all valid move into vector
			}
		}
	}
	return allValid;
}


std::pair<int, int> jiahag3::KylinOthelloAI::chooseMove(const OthelloGameState& state){
	OthelloCell Ccell;
	if(state.isBlackTurn()){		//check turn
        Ccell = OthelloCell::black;
    }else{
		Ccell = OthelloCell::white;
    }
	int bestScore = -9999;		//use this score to store the best score, check the best move
	int alpha = -999;
	int beta = 999;
	std::vector<std::pair<int, int>> Vmoves = allValidMoves(state);
	std::pair<int, int> reMove;
	for(auto move = Vmoves.begin(); move != Vmoves.end(); move++){
		std::unique_ptr<OthelloGameState> tempState = state.clone();
		tempState->makeMove(move->first, move->second);
		int value = search(*tempState, alpha, beta, 3, Ccell);		//call search function to find the best move to make
		if(value > bestScore){		//if the highest point return means this move in next several round will result a good score
			bestScore = value;
			reMove = std::make_pair(move->first,move->second);
		}
	}
	return reMove;
}


int jiahag3::KylinOthelloAI::search(OthelloGameState& state, int alpha, int beta, int depth, OthelloCell& myColor){
	OthelloCell checkCell;
	if(state.isBlackTurn()){		//check turn
		checkCell = OthelloCell::black;
	}else{
		checkCell = OthelloCell::white;
	}
	std::vector<std::pair<int, int>> tempMoves = allValidMoves(state);
	if (depth == 0){		//base case, after several time recursion, check the move return score
		return evaluation(state, myColor);
	}
	if(checkCell == myColor){	//if color is my color/turn find the maxium point
		int value = -999;
		for(int i = 0; i < tempMoves.size(); i++){
			std::unique_ptr<OthelloGameState> newState = state.clone();
			std::pair<int, int> move = tempMoves[i];
			newState->makeMove(move.first, move.second);
			value = std::max(value, search(*newState, alpha, beta, depth-1, myColor));		//find higher value and apply the alpha beta pruning
			alpha = std::max(value, alpha);
			if(beta <= alpha){
				break;
			}
		}
		return alpha;
	}else{			//if color is not my color/turn find the minimum point
		int value = 999;
		for(int i = 0; i < tempMoves.size(); i++){
			std::unique_ptr<OthelloGameState> newState = state.clone();
			std::pair<int, int> move = tempMoves[i];
			newState->makeMove(move.first, move.second);
			value = std::min(value, search(*newState, alpha, beta, depth-1, myColor));		//find lower value and apply the alpha beta pruning
			beta = std::min(value, beta);
			if(beta <= alpha){
				break;
			}	
		}
		return beta;
	}
}


int jiahag3::KylinOthelloAI::evaluation(OthelloGameState& state, OthelloCell myColor){
	int revalue = 0;
	std::unique_ptr<OthelloGameState> tempState = state.clone();
	if(myColor == OthelloCell::black){
		revalue += (tempState->blackScore()-tempState->whiteScore());
	}else{
		revalue += (tempState->whiteScore()-tempState->blackScore());
	}
	for(int x = 0; x < state.board().width(); x++){
		for(int y = 0; y < state.board().height(); y++){
			if((x == 0 || x == state.board().width()-1) && (y == 0 || y == state.board().height()-1)){	//Corner weight the highest (0,0) (7,0) (0,7) (7,7)
				if(state.board().cellAt(x,y) == myColor){
					revalue += 120;
				}else if(state.board().cellAt(x,y) != OthelloCell::empty){
				  	revalue -= 120;
				}
			}else if((x == 1 || x == state.board().width()-2) && (y == 1 || y == state.board().height()-2)){		//the internal corner weight the lowest (1,1) (6,1) (1,6) (6,6)
				if(state.board().cellAt(x,y) == myColor){
					revalue -= 40;
				}else if(state.board().cellAt(x,y) != OthelloCell::empty){
					revalue += 40;
				}
			}else if(((x == 0 || x == state.board().height()-1) && (y == 1 || y == state.board().height()-2))|| ((x == 1 || x == state.board().height()-2) && (y == 0 || y == state.board().height()-1))){
				if(state.board().cellAt(x,y) == myColor){			//the cell next to the corner is second lowest 
					revalue -= 20;
				}else if(state.board().cellAt(x,y) != OthelloCell::empty){
					revalue += 20;
				}
			}else if(((x == 0 || x == state.board().height()-1) && (y >= 2 && y <= state.board().height()-3))|| ((x >= 2 && x <= state.board().height()-3) && (y == 0 || y == state.board().height()-1))){
				if(state.board().cellAt(x,y) == myColor){			//the side are second highest
					revalue += 10;
				}else if(state.board().cellAt(x,y) != OthelloCell::empty){
					revalue -= 10;
				}
			}else{
				if(state.board().cellAt(x,y) == myColor){			//the other cells weight 5 points
					revalue += 5;
				}else if(state.board().cellAt(x,y) != OthelloCell::empty){
					revalue -= 5;
				}
			}	
		}	
	}
	return revalue;
}
