#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include "knapsack.h"
#include "item.h"

using namespace std;

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
    knapsack bag(c, 0);
    for(int i=0; i<n; ++i) {
      int p, w;  // profit, weight of each item
      cin>>w>>p;
      item tmp(p, w);
      items.push_back(tmp);
    }
    cin>>flag;
    cin>>best;
    cout<<epoch<<", "<<flag<<endl;
    
    // get the answer of partial knapsack problem
    double partial_best = 0.0;
    sort(items.rbegin(), items.rend());  // decently sorted by profit/weight
    for(vector<item>::iterator iter = items.begin(); iter != items.end(); ++iter) {
      if(bag.load(*iter)==-1) {
        // cannot fully load in
        partial_best += iter->profit * bag.capacity / iter->weight;
        break;
      }
      else {
        partial_best = bag.profit;
      }
    }
    cout<<"0-1 True: "<<best<<endl;
    cout<<"Partial Get: "<<bag.profit<<endl;
  }
  return 0;
}
