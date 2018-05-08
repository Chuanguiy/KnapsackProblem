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

  node(){
    weight=0, profit=0, level=1, flag=0;
  }
  node(int w, int v, int l, int f) {
    weight=w, profit=v, level=l, flag=f;
  }
};

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

int branchbound(vector<item> items, int n, int c) {
  int i = 1;
  queue<node> q;
  // define the level tag and live node
  node tag(-1,0,0,0), live(0,0,1,0);
  q.push(tag);
  for(;;) {
    int w = live.weight, p=live.profit;

    if(i<=n) {
      node left(w+items[i-1].weight, p+items[i-1].profit, i, 1);
      node right(w, p, i, 0);
      // load item i-1
      if(left.weight <= c) {
        // best profit always produced on the left node
        best_profit = max(best_profit, left.profit);
        q.push(left);
      }
      if(bound(items, right.profit, i, n, c-right.weight)) {
        // dont load item i-1
        q.push(right);
      }
    }

    live = q.front();
    q.pop();
    // arrive the level tag
    if(-1 == live.weight) {
      if(q.empty()) {
        break;
      }
      live = q.front();
      q.pop();
      q.push(tag);
      ++ i;
    }
  }
}

int main() {
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
  cout<<"branch bound"<<endl;
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
