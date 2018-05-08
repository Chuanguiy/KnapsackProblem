#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <queue>
#include "item.h"

using namespace std;

    class node {
    public:
      int weight;
      int profit;
      int level;
      int flag;
      int upper;

      node(){
        weight=0, profit=0, level=0, flag=0, upper=0;
      }
      node(int w, int p, int l, int f) {
        weight=w, profit=p, level=l, flag=f;
      }
      node(int w, int p, int l, int f, int u) {
        weight=w, profit=p, level=l, flag=f, upper=u;
      }
      bool operator <(const node &b) const{
        return this->upper < b.upper;
      }
    };

int best_profit = 0;

int get_upper_bound(vector<item> items, int profit, int i, int n, int c) {
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
  return profit;
}


int branchbound(vector<item> items, int n, int c) {
  priority_queue<node> q;
  // define the level tag and live node
  node live(0,0,1,0,0);
  live.upper = get_upper_bound(items, live.profit, live.level, n, c-live.weight);
  q.push(live);
  while(!q.empty()) {
    int w = live.weight, p=live.profit, i=live.level;
    if(i<=n) {
      node left(w+items[i-1].weight, p+items[i-1].profit, i+1, 1, 0);
      left.upper = get_upper_bound(items, left.profit, left.level, n, c-left.weight);
      node right(w, p, i+1, 0, 0);
      right.upper = get_upper_bound(items, right.profit, right.level, n, c-right.weight);
      // load item i-1
      if(left.weight <= c) {
        // best profit always produced on the left node
        best_profit = max(best_profit, left.profit);
        q.push(left);
      }
      if(right.upper>best_profit) {
        // dont load item i-1
        q.push(right);
      }
    }

    live = q.top();
    q.pop();
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
    branchbound(items, n, c);
    cout<<"True: "<<best<<endl;
    cout<<"Get: "<<best_profit<<endl;
    best_profit = 0;
  }
}
