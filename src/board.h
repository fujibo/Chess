#pragma once
#ifndef _BOARD_H
#define _BOARD_H

#include <iostream>
#include "te.h"
using namespace std;

enum {
	P = 1, N, B, R, Q, K, SC, LC, EM, PR
};

class Board{
protected:
  char **brd;
public:
  Board();
  ~Board();
	Board(Board &board);
	void operator=(Board &);
  void init();
  void print(ostream *os);
  char get_brd(int, int);

  bool is_checked(Te_list &mvd, bool white);
// make_new_board
  void make_board(Te &);

  void canmove_in_checked(Te_list &mvd, Te_list &mvto, bool white);

  //true includes casling
  void bp_can_move_to(char *, Te_list &, Te_list &, bool);
  void wp_can_move_to(char *, Te_list &, Te_list &, bool);

  void bcan_move_to(Te_list &, Te_list &, bool);
  void wcan_move_to(Te_list &, Te_list &, bool);

  void can_move_to(Te_list &, Te_list &, bool);
  void anmove_in_checked(Te_list &mvd, Te_list &mvto, bool white);


};

ostream &operator<<(ostream &os, Board &board);

#endif /* end of include guard: _BOARD_H */
