#ifndef KNAPSACK
#define KNAPSACK
#include "item.h"
class knapsack {
public:
  int capacity;
  int profit;


  knapsack(int c, int p):capacity(c), profit(p) {}

  // load in item and return new profit,
  //if cannot fully load in or item has been loaded, return -1
  int load(item& i) {
    if(capacity < i.weight || i.wasLoaded()) {
      return -1;
    }
    capacity -= i.weight;
    profit += i.profit;
    i.loadIn();
    return profit;
  }

  // unload item and return new profit
  int unload(item& i) {
    if(i.wasLoaded()) {
      capacity += i.weight;
      profit -= i.profit;
      i.loadOut();
      return profit;
    }
    return -1;
  }
};
#endif
