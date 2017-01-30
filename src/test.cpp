#include "board.h"
#include "think_board.h"
#include "te.h"
#include <fstream>

using namespace std;

void move_name(Board &brd, Te_list &mvd, Te &te, string *str) {
	Te_list mvto;
	bool white = (te[0] > 0) ? true : false;

	switch (te[0]) {
	case SC:
	case -SC:
		(*str) = "O-O";
		return;

	case LC:
	case -LC:
		(*str) = "O-O-O";
		return;
	case P:
	case -P:
	case PR:
	case -PR:
		if (brd.get_brd(te[3], te[4]) != 0) {
			(*str).push_back('a' + te[1]);
		}
		break;
	case EM:
	case -EM:
		(*str).push_back('a' + te[1]);
		(*str).push_back('x');
		break;
	case K:
	case -K:
		(*str).push_back('K');
		break;
	case N:
	case -N:
		(*str).push_back('N');
		break;
	case B:
	case -B:
		(*str).push_back('B');
		break;
	case R:
	case -R:
		(*str).push_back('R');
		break;
	case Q:
	case -Q:
		(*str).push_back('Q');
		break;
	}
	if ((te[0] > 1 && te[0] < 6) || (te[0] < -1 && te[0] > -6)) {
		brd.can_move_to(mvd, mvto, white);

		for (int i = 0; i < (int)mvto.size(); i++) {
			if (mvto[i][0] == te[0]) {
				if (mvto[i][3] == te[3] && mvto[i][4] == te[4]) {
					if (mvto[i][1] == te[1] && mvto[i][2] == te[2])
						continue;
					else if (mvto[i][1] != te[1] && mvto[i][2] == te[2]) {
						(*str).push_back('a' + te[1]);
					}
					else if (mvto[i][1] == te[1] && mvto[i][2] != te[2]) {
						(*str).push_back('1' + te[1]);
					}
					//cannot move to the same points
				}
			}
		}
	}

	if (brd.get_brd(te[3], te[4])!= 0) {
		(*str).push_back('x');
	}
	(*str).push_back('a' + te[3]);
	(*str).push_back('1' + te[4]);

	if (te[0] == PR || te[0] == -PR) {
		str->push_back('=');
		str->push_back('Q');
	}
	return;
}


void make_pgn(Te_list &moved, bool white) {
  Board brd;
	Te_list mvd;
  brd.init();

	ofstream ofs("pgn.pgn");
	ofs << "[Event \"PC vs PC\"]" << endl;
	ofs << "[Site \"Home\"]" << endl;
	ofs << "[Date \"2016.06.08\"]" << endl;
	ofs << "[Round \"1\"]" << endl;
	ofs << "[White \"COM1\"]" << endl;
	ofs << "[Black \"COM1\"]" << endl;
	if (white)
		ofs << "[Result \"1-0\"]" << endl;
	else
		ofs << "[Result \"0-1\"]" << endl;
	ofs << "[TimeControl \"NoLimit\"]" << endl;
	ofs << endl;

	for (int i = 0; i < (int) moved.size(); i++) {
		string te;
		int num = i / 2 + 1;
		if (i % 2 == 0) {
			ofs << num << ". ";
		}
		string str;
		move_name(brd, mvd, moved[i], &str);
		ofs << str << ' ';
		brd.make_board(moved[i]);
		mvd.push_back(moved[i]);
	}
	if (white)
		ofs << " 1 - 0" << endl << endl;
	else
		ofs << " 0 - 1" << endl << endl;
}

int main(){
  Think_Board board;
  board.init();
  std::cout << board << std::endl;
	// Think_Board foo = board;
	// cout << foo << endl;

  Te_list moved;
  bool teban = true;
	int val;

	while (board.value() < 50 && board.value() > -50){
    Te bestmv(5);
		if (board.think_rnd(DEP, teban, moved, bestmv) == 1)
			break;

		// std::vector<int> nowbestv(DEP);

		// val = board.think(DEP, teban, moved, bestmv, nowbestv);
		// cout << "end to think" << endl;
		// if (val == 200 || val == -200)
			// break;
		// cout << "value = " << val;


		teban = !teban;
    // cout << moved;
		prints_te(bestmv);
		cout << endl;

		board.make_board(bestmv);
		moved.push_back(bestmv);
    cout << board << endl;
	}

	make_pgn(moved, !teban);

  return 0;
}
