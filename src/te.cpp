#include <iostream>
#include "te.h"

using namespace std;

void prints_te(Te &te) {
	cout << (int)te[0] << (int)te[1] << (int)te[2] << (int)te[3] << (int)te[4] << ' ';
}

void prints_telist(Te_list &te_list){
  for(Te_list::iterator itr = te_list.begin(); itr != te_list.end(); itr++){
    prints_te(*itr);
  }
}
