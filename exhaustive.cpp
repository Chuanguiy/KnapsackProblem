#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdio>
#include "item.h"

using namespace std;

int best_profit = 0;

void exhaustive(vector<item> items, int c, int n, int p) {
    if(0==n || c<=0) {
      best_profit = max(best_profit, p);
      return;
    }
    if(c>=items[n-1].weight) {
      exhaustive(items, c-items[n-1].weight, n-1, p+items[n-1].profit);
    }

    exhaustive(items, c, n-1, p);
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

    exhaustive(items, c, n, 0);
    cout<<"True: "<<best<<endl;
    cout<<"Get: "<<best_profit<<endl;
  }
}
