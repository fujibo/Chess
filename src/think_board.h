#pragma once
#ifndef _THINK_BOARD
#define _THINK_BOARD

#include "board.h"
#define DEP 5
#define RANDSIZE 100

class Think_Board : public Board{
public:
  int value();
  int do_kyoku(int, bool, Te_list &);
  int think(int, bool, Te_list &, Te &, std::vector<int> &);
  int think_rnd(int, bool, Te_list &, Te &);
};

void prints_te(Te &);
#endif /* end of include guard: _THINK_BOARD */
