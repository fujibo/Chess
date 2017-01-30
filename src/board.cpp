#include <cstdio>
#include "board.h"

using namespace std;

char norm(char a) {
	return (a > 0) ? a : -a;
}

Board::Board(){
  brd = new char* [8];
  for(int i = 0; i < 8; i++){
    brd[i] = new char[8];
  }
}

Board::~Board(){
  for(int i = 0; i < 8; i++){
    delete brd[i];
  }
  delete[] brd;
}

void Board::operator=(Board &board){
		if(this == &board){return;}
		for(int i = 0; i < 8; i++){
			for(int j = 0; j < 8; j++){
					brd[i][j] = board.get_brd(i, j);
			}
		}
}

Board::Board(Board &board){
	brd = new char* [8];
  for(int i = 0; i < 8; i++){
    brd[i] = new char[8];
  }
	Board::operator=(board);
}

void Board::init(){
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      brd[i][j] = 0;
    }
  }
  brd[0][0] = brd[7][0] = R;
  brd[1][0] = brd[6][0] = N;
  brd[2][0] = brd[5][0] = B;
  brd[3][0] = Q;
  brd[4][0] = K;
  for (int i = 0; i < 8; i++) {
    brd[i][1] = P;
    brd[i][6] = -brd[i][1];
    brd[i][7] = -brd[i][0];
  }
}

void Board::print(ostream *os){
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			switch (brd[j][i]) {
			case 0:
				*os << "  \t";
				break;
			case P:
				*os << " P\t";
				break;
			case N:
				*os << " N\t";
				break;
			case B:
				*os << " B\t";
				break;
			case R:
				*os << " R\t";
				break;
			case Q:
				*os << " Q\t";
				break;
			case K:
				*os << " K\t";
				break;
			case -P:
				*os << "-P\t";
				break;
			case -N:
				*os << "-N\t";
				break;
			case -B:
				*os << "-B\t";
				break;
			case -R:
				*os << "-R\t";
				break;
			case -Q:
				*os << "-Q\t";
				break;
			case -K:
				*os << "-K\t";
				break;
			}
		}
		*os << endl << endl;
	}
	*os << endl;
}

char Board::get_brd(int x, int y){
  return brd[x][y];
}

ostream &operator<<(ostream &os, Board &board){
  board.print(&os);
  return os;
}

void Board::make_board(Te &str) {
	switch (str[0]) {
	case EM:
		brd[str[1]][str[2]] = 0;
		brd[str[3]][str[4]] = str[0];
		brd[str[3]][str[4] - 1] = 0;
		break;
	case -EM:
		brd[str[1]][str[2]] = 0;
		brd[str[3]][str[4]] = str[0];
		brd[str[3]][str[4] + 1] = 0;
		break;
	case SC:
		brd[4][0] = 0; brd[7][0] = 0;
		brd[6][0] = K; brd[5][0] = R;
		break;
	case -SC:
		brd[4][7] = 0; brd[7][7] = 0;
		brd[6][7] = -K; brd[5][7] = -R;
		break;
	case LC:
		brd[4][0] = 0; brd[7][0] = 0;
		brd[2][0] = K; brd[3][0] = R;
		break;
	case -LC:
		brd[4][7] = 0; brd[7][7] = 0;
		brd[2][7] = -K; brd[3][7] = -R;
		break;

	default:
		brd[str[1]][str[2]] = 0;
		if (str[0] == -PR)
			brd[str[3]][str[4]] = -Q;
		else
			brd[str[3]][str[4]] = (str[0] == PR) ? Q : str[0];
		break;
	}
}

// cannot avoid illegal move(attack without cheking check)
bool Board::is_checked(Te_list &mvd, bool white) {
	bool checked = false;
	if (white) {
		int row, col;
		for (row = 0; row < 8; row++) {
			for (col = 0; col < 8; col++) {
				if (brd[row][col] == K)
					break;
			}
			if (brd[row][col] == K)
				break;
		}

		Te_list bmvto;
		this->bcan_move_to(mvd, bmvto, false);//mvd is not correct, but there seems to be no matter

		//not attacked
		for (Te_list::iterator itr = bmvto.begin(); itr != bmvto.end(); ++itr) {
			if ((*itr)[3] == row && (*itr)[4] == col) {
				checked = true;
				// cout << "checked" << checked << endl;
				break;
			}
		}
		// destruct bmvto
		return checked;
	}
	else {
		int row, col;
		for (row = 0; row < 8; row++) {
			for (col = 0; col < 8; col++) {
				if (brd[row][col] == -K)
					break;
			}
			if (brd[row][col] == -K)
				break;
		}
		Te_list wmvto;
		this->wcan_move_to(mvd, wmvto, false);//mvd is not correct, but there seems to be no matter

		//not attacked
		for (Te_list::iterator itr = wmvto.begin(); itr != wmvto.end(); ++itr) {
			if ((*itr)[3] == row && (*itr)[4] == col) {
				checked = true;
				// cout << "checked" << checked << endl;
				break;
			}
		}

		// destruc wmvto
		return checked;
	}
}

void Board::canmove_in_checked(Te_list &mvd, Te_list &mvto, bool white) {
	if (white) {
		this->wcan_move_to(mvd, mvto, false);
	}
	else {
		this->bcan_move_to(mvd, mvto, false);
	}
	for (Te_list::iterator itr = mvto.begin(); itr != mvto.end(); ) {
		// prints_te(*itr);
		Board board = *this;
		Te_list next_mvd = mvd;
		// cout << "can move in checked" << endl;
		// cout << "this" << endl;
		// cout << *this;
		// cout << "copy" << endl;
		// cout << board;
		board.make_board((*itr));
		// cout << "after make board" << endl;
		// cout << *this;
		// cout << board;
		next_mvd.push_back(*itr);
		bool checked = board.is_checked(next_mvd, white);
		if (checked) {
			// itr++;
			itr = mvto.erase(itr);
		}
		else {
			itr++;
		}
		// cout << "@ the end" << endl;
		// cout << *this;
		// cout << "board" << endl;
		// cout << board;
  }
}

//point:checking mass,  brd: board at the time
void Board::wp_can_move_to(char *point, Te_list &mvd, Te_list &mvto, bool castling) {
	char piece = brd[point[0]][point[1]];
	{
		switch (piece) {
		case 0:
			return;
		case P:
			//empassan
			if (!mvd.empty() && mvd.back()[0] == -P && point[1] == 4 && (point[0] - 1 == mvd.back()[1] || point[0] + 1 == mvd.back()[1]) && mvd.back()[2] == 6 && mvd.back()[4] == 4) {
				Te str(5);
				str[0] = EM;	str[1] = point[0];	str[2] = point[1];
				str[3] = mvd.back()[1];	str[4] = mvd.back()[2] - 1;
				mvto.push_back(str);
			}
			if (brd[point[0]][point[1] + 1] == 0) {
				Te str(5);
				str[0] = (point[1] + 1 == 7) ? PR : P;	str[1] = point[0];	str[2] = point[1];
				str[3] = point[0];	str[4] = point[1] + 1;
				mvto.push_back(str);

				if (point[1] == 1 && brd[point[0]][point[1] + 2] == 0) {
					Te str(5);
					str[0] = P;	str[1] = point[0];	str[2] = point[1];
					str[3] = point[0];	str[4] = point[1] + 2;
					mvto.push_back(str);
				}
			}
			if (point[0] < 7) {
				if (brd[point[0] + 1][point[1] + 1] < 0) {//enemy
					Te str(5);
					str[0] = (point[1] + 1 == 7) ? PR : P;	str[1] = point[0];	str[2] = point[1];
					str[3] = point[0] + 1;	str[4] = point[1] + 1;
					mvto.push_back(str);
				}
			}
			if (point[0] > 0) {
				if (brd[point[0] - 1][point[1] + 1] < 0) {//enemy
					Te str(5);
					str[0] = (point[1] + 1 == 7) ? PR : P;	str[1] = point[0];	str[2] = point[1];
					str[3] = point[0] - 1;	str[4] = point[1] + 1;
					mvto.push_back(str);
				}
			}
			break;
		case N:
			for (char i = 0; i < 8; i++) {
				for (char j = 0; j < 8; j++) {
					if (norm(i - point[0]) == 1 && norm(j - point[1]) == 2) {
						if (brd[i][j] <= 0) {
							Te str(5);
							str[0] = N; str[1] = point[0]; str[2] = point[1];
							str[3] = i; str[4] = j;
							mvto.push_back(str);
						}
					}
					else if (norm(i - point[0]) == 2 && norm(j - point[1]) == 1) {
						if (brd[i][j] <= 0) {
							Te str(5);
							str[0] = N; str[1] = point[0]; str[2] = point[1];
							str[3] = i; str[4] = j;
							mvto.push_back(str);
						}
					}
				}
			}
			break;
		case Q:
		case B:
			for (int i = 1; i < 8; i++) {
				if (point[0] + i > 7 || point[1] + i > 7) break;
				if (brd[point[0] + i][point[1] + i] <= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0] + i; str[4] = point[1] + i;
					mvto.push_back(str);
					if (brd[point[0] + i][point[1] + i] != 0) break;
				}
				else break;
			}
			for (int i = 1; i < 8; i++) {
				if (point[0] + i > 7 || point[1] - i < 0) break;
				if (brd[point[0] + i][point[1] - i] <= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0] + i; str[4] = point[1] - i;
					mvto.push_back(str);
					if (brd[point[0] + i][point[1] - i] != 0) break;
				}
				else break;
			}
			for (int i = 1; i < 8; i++) {
				if (point[0] - i < 0 || point[1] - i < 0) break;
				if (brd[point[0] - i][point[1] - i] <= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0] - i; str[4] = point[1] - i;
					mvto.push_back(str);
					if (brd[point[0] - i][point[1] - i] != 0) break;
				}
				else break;
			}
			for (int i = 1; i < 8; i++) {
				if (point[0] - i < 0 || point[1] + i > 7) break;
				if (brd[point[0] - i][point[1] + i] <= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0] - i; str[4] = point[1] + i;
					mvto.push_back(str);
					if (brd[point[0] - i][point[1] + i] != 0) break;
				}
				else break;
			}
			if (piece == B) break;
		case R:
			for (int i = 1; i < 8; i++) {
				if (point[0] + i > 7) break;
				if (brd[point[0] + i][point[1]] <= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0] + i; str[4] = point[1];
					mvto.push_back(str);
					if (brd[point[0] + i][point[1]] != 0) break;
				}
				else if (brd[point[0] + i][point[1]] > 0)
					break;

			}
			for (int i = 1; i < 8; i++) {
				if (point[0] - i < 0) break;
				if (brd[point[0] - i][point[1]] <= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0] - i; str[4] = point[1];
					mvto.push_back(str);
					if (brd[point[0] - i][point[1]] != 0) break;
				}
				else if (brd[point[0] - i][point[1]] > 0)
					break;
			}
			for (int i = 1; i < 8; i++) {
				if (point[1] + i > 7) break;
				if (brd[point[0]][point[1] + i] <= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0]; str[4] = point[1] + i;
					mvto.push_back(str);
					if (brd[point[0]][point[1] + i] != 0) break;
				}
				else if (brd[point[0]][point[1] + i] > 0)
					break;
			}
			for (int i = 1; i < 8; i++) {
				if (point[1] - i < 0) break;
				if (brd[point[0]][point[1] - i] <= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0]; str[4] = point[1] - i;
					mvto.push_back(str);
					if (brd[point[0]][point[1] - i] != 0) break;
				}
				else if (brd[point[0]][point[1] - i] > 0)
					break;
			}
			break;
		case K:
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (i == 0 && j == 0) continue;
					if (point[0] + i < 0 || point[0] + i > 7 || point[1] + j < 0 || point[1] + j > 7) continue;
					if (brd[point[0] + i][point[1] + j] <= 0) {
						Te str(5);
						str[0] = K; str[1] = point[0]; str[2] = point[1];
						str[3] = point[0] + i; str[4] = point[1] + j;
						mvto.push_back(str);
					}
				}
			}
			//castling

			//about position
			bool lc = (brd[0][0] == R && brd[1][0] == 0 && brd[2][0] == 0 && brd[3][0] == 0 && brd[4][0] == K) ? true : false;
			bool sc = (brd[7][0] == R && brd[6][0] == 0 && brd[5][0] == 0 && brd[4][0] == K) ? true : false;
			if (!castling) lc = sc = false;
			if (sc == false && lc == false)
				break;

			//not attacked by pone
			if (brd[4][1] == -P || brd[5][1] == -P || brd[6][1] == -P)
				sc = false;
			if (brd[1][1] == -P || brd[2][1] == -P || brd[3][1] == -P || brd[4][1] == -P || brd[5][1] == -P)
				lc = false;

			if (sc == false && lc == false)
				break;

			//R and K didn't move
			if (!mvd.empty()) {
				for (Te_list::iterator itr = mvd.begin(); itr != mvd.end(); ++itr) {
					if ((*itr)[0] == K || (*itr)[0] == SC || (*itr)[0] == LC) {
						sc = lc = false;
						break;
					}
					if ((*itr)[0] == R) {
						if ((*itr)[1] == 0 && (*itr)[2] == 0) {
							lc = false;
						}
						else if ((*itr)[1] == 7 && (*itr)[2] == 0) {
							sc = false;
						}
					}
					if (sc == false && lc == false)	break;
				}
			}
			if (sc == false && lc == false)	break;
			Te_list bmvto;
			for (char i = 0; i < 8; i++) {
				for (char j = 0; j < 8; j++) {
					char *p = (char *)malloc(sizeof(char) * 2);
					p[0] = i; p[1] = j;
					this->bp_can_move_to(p, mvd, bmvto, false);//mvd is not correct, but there seems to be no matter
					free(p);
				}
			}
			//not attacked
			for (Te_list::iterator itr = bmvto.begin(); itr != bmvto.end(); ++itr) {
				if ((*itr)[0] < 0) {
					if ((*itr)[3] == 4 && (*itr)[4] == 0 || (*itr)[3] == 5 && (*itr)[4] == 0 || (*itr)[3] == 6 && (*itr)[4] == 0) {
						sc = false;
					}
					if ((*itr)[3] == 4 && (*itr)[4] == 0 || (*itr)[3] == 3 && (*itr)[4] == 0 || (*itr)[3] == 2 && (*itr)[4] == 0) {
						lc = false;
					}
				}
			}
			// destruct bmvto
			if (sc) {
				Te str(5);
				str[0] = SC; str[1] = str[2] = str[3] = str[4] = 0;
				mvto.push_back(str);
			}
			if (lc) {
				Te str(5);
				str[0] = LC; str[1] = str[2] = str[3] = str[4] = 0;
				mvto.push_back(str);
			}
			break;
		}
	}
}


void Board::bp_can_move_to(char *point, Te_list &mvd, Te_list &mvto, bool castling) {
	char piece = brd[point[0]][point[1]];
	{
		switch (piece) {
		case 0:
			return;
		case -P:
			//empassan
			if (!mvd.empty() && mvd.back()[0] == P && point[1] == 3 && (point[0] - 1 == mvd.back()[1] || point[0] + 1 == mvd.back()[1]) && mvd.back()[2] == 1 && mvd.back()[4] == 3) {
				Te str(5);
				str[0] = -EM;	str[1] = point[0];	str[2] = point[1];
				str[3] = mvd.back()[1];	str[4] = mvd.back()[2] + 1;
				mvto.push_back(str);
			}
			if (brd[point[0]][point[1] - 1] == 0) {
				Te str(5);
				str[0] = (point[1] - 1 == 0) ? -PR : -P;	str[1] = point[0];	str[2] = point[1];
				str[3] = point[0];	str[4] = point[1] - 1;
				mvto.push_back(str);
				if (point[1] == 6 && brd[point[0]][point[1] - 2] == 0) {
					Te str(5);
					str[0] = -P;	str[1] = point[0];	str[2] = point[1];
					str[3] = point[0];	str[4] = point[1] - 2;
					mvto.push_back(str);
				}
			}
			if (point[0] < 7) {
				if (brd[point[0] + 1][point[1] - 1] > 0) {//enemy
					Te str(5);
					str[0] = (point[1] + 1 == 0) ? -PR : -P;	str[1] = point[0];	str[2] = point[1];
					str[3] = point[0] + 1;	str[4] = point[1] - 1;
					mvto.push_back(str);
				}
			}
			if (point[0] > 0) {
				if (brd[point[0] - 1][point[1] - 1] > 0) {//enemy
					Te str(5);
					str[0] = (point[1] + 1 == 0) ? -PR : -P;	str[1] = point[0];	str[2] = point[1];
					str[3] = point[0] - 1;	str[4] = point[1] - 1;
					mvto.push_back(str);
				}
			}
			break;
		case -N:
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (norm(i - point[0]) == 1 && norm(j - point[1]) == 2) {
						if (brd[i][j] >= 0) {
							Te str(5);
							str[0] = -N; str[1] = point[0]; str[2] = point[1];
							str[3] = i; str[4] = j;
							mvto.push_back(str);
						}
					}
					else if (norm(i - point[0]) == 2 && norm(j - point[1]) == 1) {
						if (brd[i][j] >= 0) {
							Te str(5);
							str[0] = -N; str[1] = point[0]; str[2] = point[1];
							str[3] = i; str[4] = j;
							mvto.push_back(str);
						}
					}
				}
			}
			break;
		case -Q:
		case -B:
			for (int i = 1; i < 8; i++) {
				if (point[0] + i > 7 || point[1] + i > 7) break;
				if (brd[point[0] + i][point[1] + i] >= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0] + i; str[4] = point[1] + i;
					mvto.push_back(str);
					if (brd[point[0] + i][point[1] + i] != 0) break;
				}
				else break;
			}
			for (int i = 1; i < 8; i++) {
				if (point[0] + i > 7 || point[1] - i < 0) break;
				if (brd[point[0] + i][point[1] - i] >= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0] + i; str[4] = point[1] - i;
					mvto.push_back(str);
					if (brd[point[0] + i][point[1] - i] != 0) break;
				}
				else break;
			}
			for (int i = 1; i < 8; i++) {
				if (point[0] - i < 0 || point[1] - i < 0) break;
				if (brd[point[0] - i][point[1] - i] >= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0] - i; str[4] = point[1] - i;
					mvto.push_back(str);
					if (brd[point[0] - i][point[1] - i] != 0) break;
				}
				else break;
			}
			for (int i = 1; i < 8; i++) {
				if (point[0] - i < 0 || point[1] + i > 7) break;
				if (brd[point[0] - i][point[1] + i] >= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0] - i; str[4] = point[1] + i;
					mvto.push_back(str);
					if (brd[point[0] - i][point[1] + i] != 0) break;
				}
				else break;
			}
			if (piece == -B) break;
		case -R:
			for (int i = 1; i < 8; i++) {
				if (point[0] + i > 7) break;
				if (brd[point[0] + i][point[1]] >= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0] + i; str[4] = point[1];
					mvto.push_back(str);
					if (brd[point[0] + i][point[1]] != 0) break;
				}
				else break;
			}
			for (int i = 1; i < 8; i++) {
				if (point[0] - i < 0) break;
				if (brd[point[0] - i][point[1]] >= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0] - i; str[4] = point[1];
					mvto.push_back(str);
					if (brd[point[0] - i][point[1]] != 0) break;
				}
				else break;
			}
			for (int i = 1; i < 8; i++) {
				if (point[1] + i > 7) break;
				if (brd[point[0]][point[1] + i] >= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0]; str[4] = point[1] + i;
					mvto.push_back(str);
					if (brd[point[0]][point[1] + i] != 0) break;
				}
				else break;
			}
			for (int i = 1; i < 8; i++) {
				if (point[1] - i < 0) break;
				if (brd[point[0]][point[1] - i] >= 0) {
					Te str(5);
					str[0] = piece; str[1] = point[0]; str[2] = point[1];
					str[3] = point[0]; str[4] = point[1] - i;
					mvto.push_back(str);
					if (brd[point[0]][point[1] - i] != 0) break;
				}
				else break;
			}
			break;
		case -K:
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (i == 0 && j == 0) continue;
					if (point[0] + i < 0 || point[0] + i > 7 || point[1] + j < 0 || point[1] + j > 7) continue;
					if (brd[point[0] + i][point[1] + j] >= 0) {
						Te str(5);
						str[0] = -K; str[1] = point[0]; str[2] = point[1];
						str[3] = point[0] + i; str[4] = point[1] + j;
						mvto.push_back(str);
					}
				}
			}
			//castling

			//about position
			bool lc = (brd[0][7] == -R && brd[1][7] == 0 && brd[2][7] == 0 && brd[3][7] == 0 && brd[4][7] == -K) ? true : false;
			bool sc = (brd[7][7] == -R && brd[6][7] == 0 && brd[5][7] == 0 && brd[4][7] == -K) ? true : false;
			if (!castling) lc = sc = false;

			if (sc == false && lc == false)
				break;

			//not attacked by pone
			if (brd[4][6] == P || brd[5][6] == P || brd[6][6] == P)
				sc = false;
			if (brd[1][6] == P || brd[2][6] == P || brd[3][6] == P || brd[4][6] == P || brd[5][6] == P)
				lc = false;

			if (sc == false && lc == false)
				break;

			//R and K didn't move
			if (!mvd.empty()) {
				for (Te_list::iterator itr = mvd.begin(); itr != mvd.end(); ++itr) {
					if ((*itr)[0] == -K || (*itr)[0] == -SC || (*itr)[0] == -LC) {
						sc = lc = false;
						break;
					}
					if ((*itr)[0] == -R) {
						if ((*itr)[1] == 0 && (*itr)[2] == 7) {
							lc = false;
						}
						else if ((*itr)[1] == 7 && (*itr)[2] == 7) {
							sc = false;
						}
					}
					if (sc == false && lc == false)	break;
				}
			}
			if (sc == false && lc == false)	break;
			Te_list wmvto;
			for (char i = 0; i < 8; i++) {
				for (char j = 0; j < 8; j++) {
					char *p = (char *)malloc(sizeof(char) * 2);
					p[0] = i; p[1] = j;
					this->wp_can_move_to(p, mvd, mvto, false);//mvd is not correct, but there seems to be no matter
					free(p);
				}
			}
			//not attacked
			for (Te_list::iterator itr = wmvto.begin(); itr != wmvto.end(); ++itr) {
				if ((*itr)[0] > 0) {
					if ((*itr)[3] == 4 && (*itr)[4] == 7 || (*itr)[3] == 5 && (*itr)[4] == 7 || (*itr)[3] == 6 && (*itr)[4] == 7) {
						sc = false;
					}
					if ((*itr)[3] == 4 && (*itr)[4] == 7 || (*itr)[3] == 3 && (*itr)[4] == 7 || (*itr)[3] == 2 && (*itr)[4] == 7) {
						lc = false;
					}
				}
			}
			// destruct wmvto
			if (sc) {
				Te str(5);
				str[0] = -SC; str[1] = str[2] = str[3] = str[4] = 0;
				mvto.push_back(str);
			}
			if (lc) {
				Te str(5);
				str[0] = -LC; str[1] = str[2] = str[3] = str[4] = 0;
				mvto.push_back(str);
			}
			break;
		}
	}
}


void Board::wcan_move_to(Te_list &mvd, Te_list &mvto, bool castling) {
	for (char i = 0; i < 8; i++) {
		for (char j = 0; j < 8; j++) {
			char *p = (char *)malloc(sizeof(char) * 2); p[0] = i; p[1] = j;
			this->wp_can_move_to(p, mvd, mvto, castling);
			free(p);
		}
	}
}

void Board::bcan_move_to(Te_list &mvd, Te_list &mvto, bool castling) {
	for (char i = 0; i < 8; i++) {
		for (char j = 0; j < 8; j++) {
			char *p = (char *)malloc(sizeof(char) * 2); p[0] = i; p[1] = j;
			this->bp_can_move_to(p, mvd, mvto, castling);
			free(p);
		}
	}
}

void Board::can_move_to(Te_list &mvd, Te_list &mvto, bool white) {
	if (this->is_checked(mvd, white)) {
		this->canmove_in_checked(mvd, mvto, white);
	}
	else {
		if (white)
			this->wcan_move_to(mvd, mvto, true);
		else
			this->bcan_move_to(mvd, mvto, true);
	}
}
