#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include "item.h"

using namespace std;

int best_profit = 0;


bool bound(vector<item> items, int profit, int i, int n, int c) {
  for(; i<n; ++i) {
    c -= items[i].weight;
    if(c > 0) {
      profit += items[i].profit;
    }
    else {
      c += items[i].weight;
      profit += items[i].profit * c / items[i].weight + 1;  // get the upper int
      break;
    }
  }
  return profit > best_profit;
}

void backtrack(vector<item> items, int c, int n, int p, int i) {
    best_profit = max(best_profit, p);
    // a naive and relaxed bound, the rest profit plus current profit cannot get a better solution
    // then cut the tree
    if(i==n || c<=0) {
      return;
    }
    ++ i;
    if(c>=items[i-1].weight) {
      backtrack(items, c-items[i-1].weight, n, p+items[i-1].profit, i);
    }
    if(bound(items, p, i, n, c)) {
      backtrack(items, c, n, p, i);
    }

}

int main() {
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
  int epoches;
  string flag;
  cin>>epoches;
  for(int epoch=1; epoch<=epoches; ++epoch) {
    int n, c, best;  // number, capacity
    vector<item> items;
    cin>>flag;
    cout<<epoch<<", "<<flag<<endl;
    cin>>c>>n;
    for(int i=0; i<n; ++i) {
      int p, w;  // profit, weight of each item
      cin>>w>>p;
      item tmp(p, w);
      items.push_back(tmp);
    }
    cin>>flag;
    cin>>best;
    cout<<epoch<<", "<<flag<<endl;
    sort(items.rbegin(), items.rend());
    backtrack(items, c, n, 0, 0);
    cout<<"True: "<<best<<endl;
    cout<<"Get: "<<best_profit<<endl;
    best_profit = 0;
  }
}
