#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "knapsack.h"
#include "item.h"

using namespace std;

int ** new_array2d(int m, int n) {
  int **p = new int*[m];
  for(int i=0; i<m; ++i) {
    p[i] = new int[n];
    memset(p[i], 0, sizeof(int)*n);
  }
  return p;
}


void del_array2d(int** p, int m, int n) {
  for(int i=0; i<m; ++i) {
    delete[] p[i];
  }
  delete[] p;
}


int dynamic_try(vector<item> &items, vector<item> &bag, int c, int minus){
  int n = items.size(), best_profit;
  // get the best solution
  int **dp = new_array2d(n+1, c+1);
  for(int i=1; i<=c; ++i) { dp[n][i] = 0;}

  for(int i=n-1; i>=0; --i) {
    dp[i][0] = 0;
    for(int j=1; j<=c; ++j) {
      if(items[i].weight <= j) {
        dp[i][j] = max(dp[i+1][j], dp[i+1][j-items[i].weight]+items[i].profit);
      }
      else {
        dp[i][j] = dp[i+1][j];
      }
    }
  }
  best_profit = dp[0][c];
  // is a better solution or not
  if(best_profit > minus) {
    int capacity;
    // find the sum of weight
    for(int j=c; j>0; --j) {
      if(dp[0][c] != dp[0][j]){
        capacity = j+1;
        break;
      }
    }
    for(int i=0; i<n; ++i) {
      if(dp[i][capacity] != dp[i+1][capacity]){
        bag.push_back(items[i]);
        capacity -= items[i].weight;
      }
    }
  }
  del_array2d(dp, n+1, c+1);

  return best_profit - minus;
}


int monte(vector<item> &items, vector<item> &bag, int profit, int capacity) {
  for(int i=0; i<bag.size(); ++i) {
    capacity -= bag[i].weight;
  }
  srand(unsigned(time(0)));
  for(int t=0; t<10; ++t) {
    int index = int(rand() % (bag.size()-1));
    vector<item>::iterator iter = bag.begin() + index;
    capacity += iter->weight;
    // apply dynamic programming to rest items with rest capacity
    int addment = dynamic_try(items, bag, capacity, iter->profit);
    if(addment>0) {
      bag.erase(iter);
      return profit+addment;
    }
  }
  return profit;
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

    vector<item> knap;
    int carry = 0, best_profit=0;
    sort(items.begin(), items.end());  // decently sorted by profit/weight
    // begin with greedy method
    while(carry<c){
      item tmp = items.back();
      if(carry+tmp.weight > c) {
        break;
      }
      knap.push_back(tmp);
      carry += tmp.weight;
      best_profit += tmp.profit;
      items.pop_back();
    }
    for(int batch=0; batch<10; ++batch) {
      best_profit = monte(items, knap, best_profit, c);
      cout<<"batch "<<batch<<","<<best_profit<<endl;
    }
    cout<<"0-1 True: "<<best<<endl;
    cout<<"Partial Get: "<<best_profit<<endl;
  }
  return 0;
}
