#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdio>
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

vector<item> back_dp(vector<item> items, int **dp, int n, int c) {
  int capacity;
  vector<item> bag;
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
  return bag;
}


int dynamic(vector<item> items, int c){
  int n = items.size(), best_profit;
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
  back_dp(items, dp, n, c);
  del_array2d(dp, n+1, c+1);
  return best_profit;
}

int dynamic_scroll(vector<item> items, int c) {
  int n = items.size(), best_profit;
  int **dp = new_array2d(2, c+1);
  for(int i=0; i<=c; ++i) { dp[0][i] = 0; dp[1][i] = 0;}
  int flag = 0;  // true - dp[0], false - dp[1]
  for(int i=n-1; i>=0; --i) {
    flag = (flag==1)?0:1;  // reverse
    for(int j=1; j<=c; ++j) {
      if(items[i].weight <= j) {
        dp[flag][j] = max(dp[1-flag][j], dp[1-flag][j-items[i].weight]+items[i].profit);
      }
      else {
        dp[flag][j] = dp[1-flag][j];
      }
    }
  }
  best_profit = dp[flag][c];
  del_array2d(dp, 2, c+1);
  return best_profit;
}


int dynamic_single(vector<item> items, int c) {
  int n = items.size(), best_profit;
  int *dp = new int[c+1];
  for(int i=0; i<=c; ++i) { dp[i] = 0;}
  for(int i=n-1; i>=0; --i) {
    for(int j=c; j>0; --j) {
      if(items[i].weight <= j) {
        dp[j] = max(dp[j], dp[j-items[i].weight]+items[i].profit);
      }
    }
  }
  best_profit = dp[c];
  delete[] dp;
  return best_profit;
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

    cout<<"True: "<<best<<endl;
    best = dynamic_single(items, c);
    cout<<"Get: "<<best<<endl;
  }
  return 0;
}
