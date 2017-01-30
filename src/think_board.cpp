#include <cmath>
#include "think_board.h"

using namespace std;

int Think_Board::value() {
	int val = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			switch (brd[i][j]) {
			case N:
				val += 3;
				break;
			case -N:
				val -= 3;
				break;
			case R:
				val += 5;
				break;
			case -R:
				val -= 5;
				break;
			case Q:
				val += 9;
				break;
			case -Q:
				val -= 9;
				break;
			case K:
				val += 100;
				break;
			case -K:
				val -= 100;
				break;
			default:
				val += brd[i][j];
			}
		}
	}
	return val;
}

int Think_Board::do_kyoku(int depth, bool white, Te_list &mvd) {
	int v = this->value();
	if (v > 50) return 1000;
	else if (v < -50) return -1000;
	else if (depth == 0){
		return v;
	}
	else{
		Te_list mvton;
		this->can_move_to(mvd, mvton, white);
		if (mvton.empty())
			return (white)? -1000 : 1000;
		Te te(5);
		int te_num = rand() % mvton.size();
		te = mvton[te_num];
		Te_list mvdn = mvd;
		mvdn.push_back(te);
		this->make_board(te);
		return this->do_kyoku(depth - 1, !white, mvdn);
	}
}

int Think_Board::think_rnd(int depth, bool white, Te_list &mvd, Te &bestmv) {
	Te_list mvto;
	this->can_move_to(mvd, mvto, white);
	prints_telist(mvto);
	if (mvto.empty())
		return 1;
  std::vector<int> sum(mvto.size());
	for (int j = 0; j < (int) mvto.size(); j++) {
		Te_list next_mvd;
		next_mvd = mvd;
		next_mvd.push_back(mvto[j]);
    Think_Board brdn = *this;
		// next board
		brdn.make_board(mvto[j]);

		// after one move
		sum[j] = 0;
		for (int i = 0; i < RANDSIZE; i++) {
			Te_list next_mvdn;
			next_mvdn = next_mvd;
      Think_Board next_brdn = brdn;

			sum[j] += next_brdn.do_kyoku(depth, !white, next_mvdn);
      // destruct tmp_brdn
			// destruct next_mvdn
		}
    // destruct brdn
		// error?
		// destruct next_mvd
	}
	int max_i = rand() % mvto.size();

	if (white) {
		for (int i = 0; i < (int)mvto.size(); i++) {
			if (sum[max_i] < sum[i])
				max_i = i;
		}
	}
	else {
		for (int i = 0; i < (int) mvto.size(); i++) {
			if (sum[max_i] > sum[i])
				max_i = i;
		}
	}
	bestmv = mvto[max_i];
	// destruct mvto
	// mvto.clear();
	return 0;
}


int Think_Board::think(int depth, bool white, Te_list &mvd, Te &bestmv, std::vector<int> &nowbestv) {
	if (depth == 0) {
		return this->value();
	}
	else {
		Te_list mvto;
		this->can_move_to(mvd, mvto, white);

		int bestval = (white) ? -200 : 200;
		// cannot move (stale mate)
		if (mvto.empty()) {
			return bestval;
		}

		for (int j = 0; j < (int)mvto.size(); j++) {
			// next state mvd
      Te_list next_mvd;
			next_mvd = mvd;
			next_mvd.push_back(mvto[j]);
			Think_Board brdn;
			brdn.make_board(mvto[j]);

			int tmp = brdn.think(depth-1, !white, next_mvd, bestmv, nowbestv);

			if (depth == DEP) {
				cout << "move ";
				prints_te(mvto[j]);
				cout << "value = " << tmp;
				cout << endl;
			}

			if (j == 0) {
				nowbestv[depth - 1] = tmp;
				bestval = tmp;
				if (depth == DEP) {
          bestmv = mvto[j];
					cout << "now best move ";
					prints_te(bestmv);
					cout << endl;
				}
			}
			else {
				if (white) {
					if (bestval < tmp) {
						bestval = tmp;
						if (depth != DEP) {
							if (bestval > nowbestv[depth])
								break;
						}
						else {
							bestmv = mvto[j];
							cout << "now best move ";
							prints_te(bestmv);
							cout << endl;
						}
					}
				}
				else {
					if (bestval > tmp) {
						bestval = tmp;
						if (depth != DEP) {
							if (bestval < nowbestv[depth])
								break;
						}
						else {
							bestmv = mvto[j];
							cout << "now best move ";
							prints_te(bestmv);
							cout << endl;
						}
					}
				}
				if (j == (int) mvto.size()-1) {
					nowbestv[depth - 1] = bestval;
				}
			}
      // destruct brdn
		}
		// free mvto
		return bestval;
	}
}
