### 摘要

​	背包问题是一类组合优化的NP完全问题，给定一组具有一定质量和价值的物品，在限定的总重量内，选择出使得总价值最高的几种物品。相似的问题经常出现在组合数学，密码学，应用数学等领域。本文结合本学期在算法课程上学习到的算法，针对背包问题，展开论述。

​	本文的组织结构如下：

​	依次讨论穷举法，贪心算法，动态规划算法，回溯算法，分支限界法，线性规划算法，蒙特卡罗方法，归约算法，分别描述其算法思想，将其应用于对背包问题的求解，并给出c++实现；对于动态规划算法，讨论其优化算法，在只需求得最大收益的情景下，可将空间复杂度降至$O(W)$，对于蒙特卡罗方法，本文设计一种以贪心策略开始，以动态规划方法迭代最优解的方法。





### 问题定义

####0-1背包问题

#####输入

> 物品的重量序列$W$和价值序列$P$，物品个数$n$，背包总容量$c$，$W=\{w_1, w_2, ..., w_n\}$， $P=\{p_1, p_2, ..., p_n\}$，其中$w_i$，$p_i$，$n$，$c$都是int32范围内的整数。

#####输出

> 布尔型序列X，$X=\{x_1, x_2, ..., x_n\}$，其中$x_i$表示第$i$个物品是否被放入背包；
>
> 在取得物品总重不高于背包总容量时取得的最大收益p。    

##### 公式表示

> max $\sum\limits_{i=1}^{n} x_ip_i $
>
> s.t. $\sum\limits_{i=1}^{n} x_iw_i\leq c $
>
> ​      $x_i=\{0, 1\}, i=1,2,...,n$

#### 部分背包问题

##### 输入

> 物品的重量序列$W$和价值序列$P$，物品个数$n$，背包总容量$c$，$W=\{w_1, w_2, ..., w_n\}$， $P=\{p_1, p_2, ..., p_n\}$，其中$w_i$，$p_i$，$n$，$c$都是int32范围内的整数。

##### 输出

> 整型序列X，$X=\{x_1, x_2, ..., x_n\}$，其中$x_i, (0\leq x_i\leq w_i)$表示第$i$个物品被放入背包的重量；
>
> 在取得物品总重不高于背包总容量时取得的最大收益p。

##### 公式表示

> max $\sum\limits_{i=1}^{n} x_ip_i $
>
> s.t. $\sum\limits_{i=1}^{n} x_i\leq c $
>
> ​      $$0\leq x_i\leq w_i, i=1,2,...,n$$

### 准备工作

​	本报告中提及的所有算法，均使用c++实现并测试，代码测试标准模板如下：

```c++
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
    best = dynamic(items, n, c);
    cout<<"Get: "<<best<<endl;
  }
  return 0;
}
```
​	其中，*input.txt*文件中包含5组测试样例，五组样本规模分别是5，8， 10， 100， 100，如下是第一组测试点:

```c++
in  // 输入标志
100 5  // 背包重量，物品个数
77 92  // weight profit
22 22
29 87
50 46
99 90
out
133  // 最大收益
```
​	本文展示代码使用自定义的item类表示物品：

```c++
class item {
private:
  double pwRatio;
public:
  int profit;
  int weight;
  bool loaded;
  item(int p, int w):profit(p), weight(w), loaded(0) {
    pwRatio = double(p) / w;
  }
  bool wasLoaded() {return true == loaded;}
  bool operator <(item b) {
    return this->pwRatio < b.pwRatio;
  }
};
```
### 算法讨论

#### 穷举法

#####算法思想

> 穷举法是一种**朴素而强大**的算法，算法简单直观易于实现，通过某些方法（循环，图搜索）遍历解空间寻找可行解。
>
> 将0-1背包问题的求解看做对X序列的穷举，对于第i个物品，$x_i$只有0或1两种取值，故可以使用穷举法，在$2^n$个X序列中，选出具有最大收益的那一组。

#####问题求解

> 可知，0-1背包问题的解空间是高度为n的完全二叉树，通过树的深度优先遍历（先序，中序，后序）可遍历所有可行解，并通过一组记录变量得到最优解。

```c++
int best_profit = 0;
// DFS version
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
```
> 或通过图的广度优先遍历求解。定义树节点如下：

```c++
class node {
public:
  int weight;  // current weight of knapsack
  int profit;  // current profit we get
  int level;  // the level of the solution tree
  int flag;  // the level-th item was loaded or not
  node(){
    weight=0, profit=0, level=0, flag=0;
  }
  node(int w, int p, int l, int f) {
    weight=w, profit=p, level=l, flag=f;
  }
};
```
>*node*中，*weight*和*profit*表示从根节点到当前节点的重量和收益，level表示当前层数，对应物品序列中的第level个物品，flag表示该物品是否被装入背包；
>
>定义一个标志节点*tag*表示解空间树的层结束标志，活节点*live*记录树中节点的信息。

```c++
int branchbound(vector<item> items, int n, int c) {
  int i = 1;
  queue<node> q;
  // define the level tag and live node
  node tag(-1,0,0,0), live(0,0,1,0);
  q.push(tag);
  for(;;) {
    int w = live.weight, v=live.profit;
    node tmp(w, v, i, 0);

    if(i<=n) {
      // dont load item i-1
      q.push(tmp);
      // load item i-1
      if(live.weight+items[i-1].weight <= c) {
        tmp.weight += items[i-1].weight;
        tmp.profit += items[i-1].profit;
        best_profit = max(best_profit, tmp.profit);
        q.push(tmp);
      }
    }
    live = q.front();q.pop();
    // arrive the level tag
    if(-1 == live.weight) {
      if(q.empty()) {
        break;
      }
      live = q.front();q.pop();
      q.push(tag);
      ++ i;
    }
  }
}
```
##### 小结

> 穷举法的两种遍历解空间的方式，对应于图的深度优先搜索与广度优先搜索两种算法，相应的，略加改动，可以改造成回溯算法与分支限界法提升效率。显然的，穷举法的时间复杂度为$O(2^n)$，基于深度优先搜索的穷举法空间复杂度为$O(1)$，基于广度优先搜索的穷举法空间复杂度为$O(2^n)$。

#### 贪心算法

#####算法思想

> 贪心算法是每一步都做出当前最优的决策，总是做出局部最优的选择，并寄希望于能通过一系列的局部最优决策得到最终的全局最优解；
>
> 贪心算法并不保证得到最优解，但是一旦证明贪心策略成立且正确，就是一种高效的算法。

##### 部分背包贪心策略

> 当0-1背包问题中的物品可以被分割并部分地装入背包时，0-1背包问题就转化为部分背包问题，满足贪心算法的最优子结构性质。
>
> 根据物品的单位价值(profit/weight)降序排序，尽可能将单位价值较高的产品装入背包中，可得到最大收益。

```c++
  double partial_best = 0.0;
  sort(items.rbegin(), items.rend());  // decently sorted by profit/weight
  for(vector<item>::iterator iter = items.begin(); iter != items.end(); ++iter) {
    c -= iter->weight;
    if(c > 0) {
      partial_best += iter->profit;
    }
    else {
      c += iter->weight;
      partial_best += double(iter->profit) * c / iter->weight;
      break;
    }
  }
```
##### 0-1背包贪心策略近似算法

> 记原问题最大收益为opt，通过基于贪心策略近似算法得到近似解为app。
>
> 记$P_k=\sum\limits_{i=1}^{k}p_i$，$W_k=\sum\limits_{i=1}^{k}w_i$直观地，我们希望在$k$满足$W_k\leq c \leq W_{k+1}$时，将$P_k$作为最优解的一个近似，在某些极端情况下，比如$p_1=1, w_1=1, p_2=100, w_2=100, n=2, c=100$，得到近似解$app=1$，最优解$opt=100$，这显然是无法接受的。
>
> 故而，启发式的，另近似解为$max\{P_k,\;p_{k+1}\}$，在最终决策时，与无法装入背包的第一件物品的收益$p_{k+1}$比较，选择较大的一个作为最终决策，显然的，$\sum\limits_{i=1}^{k}p_i \leq opt \leq \sum\limits_{i=1}^{k+1}p_i$，因此有，$opt\leq 2app$，这是一个可以接受的近似比例。
>
> ​	$app=max\{p_{k+1},\; \sum\limits_{i=1}^{k}p_i\}$
>
> ​	$s.t.\; \sum\limits_{i=1}^{k}w_i \leq c \leq \sum\limits_{i=1}^{k+1}w_i$
>
> 

```c++
for(int i=0; i<items.size(); ++i) {
  c -= items[i].weight;
  if(c > 0) {
    profit += items[i].profit;
  }
  else {
    profit = max(profit, items[i].profit);
    break;
  }
}
```
##### 推广的贪心算法

> + 将所有物品分为两组，价值不超过a的与价值超过a的各为一组，a是一个预设的常数。对于任一可行解，价值超过a的最多有$opt/a \leq 2C_G /a$个，因此可以在第二组中考虑物品数不超过$2C_G/a$的子集。
> + 运行贪心策略近似算法得到近似解$C_G$，另$a=\delta C_G,\; 0<\delta<1$
> + 在第二组中考虑物品数不超过$2/\delta$的所有子集，对于每个子集，在第一组中用贪心策略求得近似解，在不超过$n^{2/\delta}$的近似解中求最大的。

推广的贪心算法可以再$O(n^{1+2/\delta})$时间内求得近似解$C_{GG}$，满足$opt \leq (1+\delta)C_{GG}$。

##### 小结

> 贪心算法的时间复杂度为$O(n\log_2n)$，空间复杂度为$O(1)$，在可以找到合适的贪心策略的情况下，贪心算法是一个很强大的算法。

#### 动态规划

##### 算法思想

> 动态规划与分治算法相似，都是将原问题拆分成一系列的子问题，并通过组合子问题的解来构造原问题的解，就像它的名字一样，dynamic programming，动态规划是一种表格法，在不同的子问题之间具有重叠的情况下，借助一些辅助空间保存公共问题的解，避免重复计算，来换取时间上的效率。

##### 状态转移方程

>初始状态<br>
>$P(1; c) =\begin{equation}  \left\{              \begin{array}{lr}              0,  & w_1 > c.\\              p_1, & w_1 <= c.\\                \end{array}  \right.  \end{equation}$  
>
>状态转移<br>
>
>$P(i; c) =\begin{equation}  \left\{  \begin{array}{lr}   P(i-1; c),  & w_i > c.\\  max\{P(i-1;c), P(i-1;c-w_i)+p_i\}, & w_i <= c.\\     \end{array}  \right.  \end{equation}$  
>

#####正确性证明

>如果第$n$件物品不放入背包，则可取得的最优解等价于，前$n-1$件物品在背包承重为$c$的情况下取得的最优解$bp$；
>
>如果第$n$件物品放入背包，则可取得的最优解等价于，前$n-1$件物品在背包承重为$c-w_n$的情况下取得的最优解$bp+p_n$；

##### 朴素算法

> 根据状态转移方程构造辅助空间 $dp[n+1][c+1]$

```c++
int dynamic(vector<item> items, int n, int c){
  int **dp = new_array2d(n+1, c+1);  // self-define function
  for(int i=1; i<=c; ++i) { dp[n][i] = 0;}  // no item
  for(int i=n-1; i>=0; --i) {
    dp[i][0] = 0;  // capacity is 0
    for(int j=1; j<=c; ++j) {
      if(items[i].weight <= j) {
        // transfer
        dp[i][j] = max(dp[i+1][j], dp[i+1][j-items[i].weight]+items[i].profit);
      }
      else {
        dp[i][j] = dp[i+1][j];
      }
    }
  }
  int best_profit = dp[0][c];
  back_dp(items, dp, n, c);
  del_array2d(dp, n+1, c+1);  // self-define function
  return best_profit;
}
```
> 根据$dp$数组反向传播得到放入背包的物品序列

```c++
vector<item> back_dp(vector<item> items, int **dp, int n, int c) {
  int capacity;
  vector<item> bag;
  // find the turely weight used
  for(int j=c; j>0; --j) {
    if(dp[0][c] != dp[0][j]){
      capacity = j+1;
      break;
    }
  }
  // back propagation
  for(int i=0; i<n; ++i) {
    if(dp[i][capacity] != dp[i+1][capacity]){
      bag.push_back(items[i]);
      capacity -= items[i].weight;
    }
  }
  return bag;
}
```
#####滚动数组优化

> 从上述代码可以看出，$dp[i][j]$仅与$dp[i+1][j]$，$dp[i+1][j-w_i]$有关，故而可以设置dp数组为$dp[2][c+1]$，依次在$dp[0]$与$dp[1]$之间滚动更新，将空间复杂度降低到$O(c)$，在n很大的情况下，可大幅度降低算法的空间复杂度。
>
> 设置滚动标志flag，每一轮迭代翻转flag，滚动推导$dp[0][j]$与$dp[1][j]$。

```c++
int dynamic_scroll(vector<item> items, int c) {
  int n = items.size(), best_profit;
  int **dp = new_array2d(2, c+1);
  for(int i=0; i<=c; ++i) { dp[0][i] = 0; dp[1][i] = 0;}
  int flag = 0;  // true - dp[0], false - dp[1]
  for(int i=n-1; i>=0; --i) {
    flag = (flag==1)?0:1;  // reverse flag
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
```
##### 一维数组实现

> 从滚动数组中，我们进一步观察，$dp[i][j]$仅与$dp[i+1][j-items[i].weight]$和$dp[i+1][j]$相关，只要保证$dp[i+1][j-items[i].weight]$在$dp[i+1][j]$之后求出，在将$dp[i+1][j]$更新为$dp[i][j]$之后，不提前改变$dp[i][j-items[i].weight]$，$dp[i][j]$即可正常实现状态转移，所以，只需保持以递减的顺序迭代重量j，即可将辅助空间$dp$变为一维。

```c++
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
```
#####小结

> 0-1背包动态规划算法的时间复杂度是$O(nc)$，空间复杂度也是$O(nc)$。动态规划的空间优化算法无法从dp数组倒推得到序列X，在仅考虑最优收益时可将空间复杂度降低至$O(c)$，在需考虑物品序列时，由于x的取值只能是0或1，可以考虑位图法，用n*c bit的空间存储动规过程中的物品放入过程，将使用的辅助空间规模降低至 $size(bool)/size(int)$。
>
> 可以看到，动态规划是一种非常强大的算法，在满足最优子结构与子问题重叠两种性质的时候，可以考虑动态规划算法。

#### 回溯法

#####算法思想

>对于一个集合S的排列组合问题，可以将解空间看做是深度为n的完全二叉树，回溯算法以深度优先遍历的方式遍历解空间，企图得到一系列的可行解，在可行解中找到最优解，并通过一些约束函数剪掉多余分支，降低时间开销。
>
>明显的，回溯法是在穷举法的基础上进行的启发式的改进，约束函数是否足够紧凑是影响回溯性能的重要因素。设0-1背包的最优解为$opt$，从贪心算法出发，将0-1背包看成部分背包，可得到一个近似解$app$，$opt\leq app$，故而$app$是$opt$的上界。
>
>将物品根据单位价值降序排序，在解空间树的第$k$层（对应第$k$个物品），记当前收益为$P_k=\sum\limits_{i=1}^k x_ip_i$，当前全局最佳收益为$best$，背包剩余空间为$c$，贪心策略在剩余物品和剩余空间上得到近似上界$app_{ic}$，则约束函数为：
>
>​	$P_k+app_{ic} \geq best$

#####问题求解

```c++
bool bound(vector<item> items, int profit, int i, int n, int c, int best_profit) {
  for(; i<n; ++i) {  // get app[i][c] + profit
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
```
```c++
void backtrack(vector<item> items, int c, int n, int p, int i) {
    best_profit = max(best_profit, p);
    // then cut the tree
    if(0==n || c<=0  || !bound(items, p, i, n, c, best_profit)) {
      return;
    }
    // load 
    if(c>=items[n-1].weight) {
      backtrack(items, c-items[n-1].weight, n-1, p+items[n-1].profit, i+1);
    }
    backtrack(items, c, n-1, p, i+1);  // not load
}
```
##### 小结

> 回溯法在穷举法的基础上进行了启发式的改进，剪去多余分支，在约束函数紧凑的情况下，效率较高。本文使用的约束函数的时间复杂度是$O(n)$，遍历解空间的时间复杂度是$O(2^n)$，故而回溯法的最坏时间复杂度是$O(n2^n)$。

#### 分支限界法

##### 算法思想

> 和回溯法类似的，分支限界法是在广度优先搜索(或最大效益优先)的基础上进行了启发式的改动，适用于求解组合优化问题。在基于最大效益优先的搜索方式上，分支限界法往往会比回溯法更快的得到最优解。

#####朴素分支限界法

> 一种朴素的分支限界法是基于广度优先搜索，并使用限界函数决定是否拓展解空间树，与回溯法基本相同，不再赘述；

#####基于优先队列优化的分支限界法

> 这里，我们讨论一个基于优先队列优化的分支限界法，沿用回溯法中使用的约束函数，拓展穷举法中定义的树节点，将树节点的上界作为节点的优先级，上界越高越优先被扩展。

```c++
class node {
public:
  int weight, profit, level, flag, upper;
  
  node(){
    weight=0, profit=0, level=0, flag=0, upper=0;
  }
  node(int w, int p, int l, int f) {
    weight=w, profit=p, level=l, flag=f;
  }
  node(int w, int p, int l, int f, int u) {
    weight=w, profit=p, level=l, flag=f, upper=u;
  }
  bool operator <(node b) {
    return this->priority < b.priority;
  }
};
```
```c++
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
```
```c++
int branchbound(vector<item> items, int n, int c) {
  priority_queue<node> q;
  // define the root
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
```
##### 小结

> 基于优先队列优化的分支限界法时间复杂度与空间复杂度依然不会降低，但是程序的运行时间会显著下降，在BFS基础上的剪枝优化策略在本报告使用的5组测试样例中，均无法在1min内给出后两组样本数为100的测试点的解，基于优先队列优化的，按照贪心上界优先遍历的剪枝算法，在1s内可以得到最优解。
>
> 原因分析：
>
> 根据三种遍历解空间的方式，给出简单的原理分析。解空间的**最左分支是贪心算法的近似解**，最优解一定产生于**某个树节点的左分支**，基于这两点，可以从算法遍历解空间的顺序给出对比分析。
>
> + 基于树的先序遍历的方式(DFS)，在items根据单位价值降序排序的情况下，得出的**第一个可行解是贪心算法近似解**，大多数情况下，会是最优解的良好近似，将其用于约束函数，将会剪掉大部分分支；
> + 基于树的层次遍历的方式(BFS)，约束函数过于松弛，大部分分支将被保留，与穷举法的性能差距不大；
> + 基于贪心上界优先遍历的剪枝算法，得到的第一个可行解同样是贪心近似，保证减去尽可能多的分支，与基于DFS的方式相比，更优先于拓展有可能得到最优解的分支，略优于回溯法。

#### 线性规划

##### 线性规划定义与解法

> 以下内容整理自理学院张薇教授编写的《最优化方法》与清华大学出版社译作《凸优化》。

​	一个线性规划问题的标准形为:

​		$min\; f(\vec x) =\vec c^T \vec x$ 

​		$s.t.\; A\vec x=\vec b$

​        		 $\vec x \geq \vec 0$

​	若$A_{m\times n}$包含$m$个列向量可构成单位矩阵，则上述线性规划是可称为线性规划典范形式；

​	对于一个标准形线性规划，可将$A_{m\times n}$分解为$(B_{m\times m}, N_{m\times(n-m)})$， 相应的，$\vec x^T$分解为基变量与非基变量$(\vec x_B^T, \vec x_N^T)$，$\vec c^T$对应分为$(\vec c_B^T, \vec c_N^T)$，则有等价线性规划问题：

​		$min\; \vec c_B^T \vec x_B + \vec c_N^T \vec x_N$

​		$s.t.\; B\vec x_B+N\vec x_N=\vec b$

​	由线性规划的定义(非基向量为$\vec 0$)可得，基本容许解 $\bar x=\left(\begin{array}{c}B^{-1}\vec b \\ \vec 0 \end{array}\right)$，基本容许解的目标函数值$\bar z=\vec C_B^TB^{-1}\vec b$，对于任意容许解$\vec x$，有$A\vec x=\vec b$，即$B\vec x_B+N\vec x_N=\vec b$，则$\vec x_B=B^{-1}[\vec b-N\vec x_N]$，将$\vec x_B$代入$\vec z=\vec c_B^T \vec x_B + \vec c_N^T \vec x_N$，可得$\vec z=\bar z-[\vec c^T_BB^{-1}N-\vec c_N^T]\vec x_N$。

​	验证关于$B$的基本容许解是最优解，即$B$是最优基：*

> 引入$B$的判别向量$\vec \sigma$，$\vec \sigma ^T=\vec c_B^T B^{-1}N - \vec v_N^T$，当$\vec \sigma \leq \vec0$时，$B$对应的基本容许解$\bar x$就是最优解。

​	故而，线性规划的过程可以描述为

> 将约束矩阵$A$划分为基$B$与非基$N$，验证$B$对应的基本容许解$\bar x$是否是最优解，若$\bar x$不是最优解，迁移至另一组基$B'$，直至找到最优基与最优解。

​	根据Gauss-Jordan方程组，实现$B \mapsto B'$的基迁移

> 为了实现在基之间高效的迁移，需满足容许性条件与下降性条件：
>
> + 容许性条件，$B^{-1}\vec b \geq \vec 0$
>
>   >这要求我们判断迁移基时的主元所在列$l$满足$b_k/a_{kl}$比值最小(主元k被l替换，l成为新的主元)，在更新主元之后，不会导致任一行$i$中存在$b_i-\dfrac {b_k*a_{ii}} {a_{kl}} < 0$
>
> + 下降性条件，$B'$作为新基会取得更优目标函数值
>
>   > 主元更新后，主元对应的判别数$\sigma$为正数

​	单纯形表法，与动态规划相似的，programming是一种表上作业法，将约束方程表示成表格的形式，即

​		$\left(\begin{array}{ccc}B & N &\vec b \\ -\vec c_B^T& -\vec c_N^T& 0\end{array}\right)$

​	对表格进行行变换$B^{-1} \times r_1,\;\vec c_B^Tr_1+r2$，转化成如下形式

​		$\left(\begin{array}{ccc}I & B^{-1}N &B^{-1}\vec b \\ \vec 0& \vec \sigma_N^T& \bar z\end{array}\right)$

​	其中$\vec \sigma_N^T$是非基变量的判别数，$B^{-1}\vec b$是基变量取值，$\bar z$是当前基变量对应的基本容许解，在变换后的单纯形表上进行基变换，可得到最优解（或无解）。



#####0-1背包线性规划标准形

> 对以下线性规划问题求解，作为0-1背包问题的近似解，
>
> max $\sum\limits_{i=1}^{n} x_ip_i $
>
> s.t. $\sum\limits_{i=1}^{n} x_iw_i= c $
>
> ​      $0\leq x_i\leq 1, i=1,2,...,n$
>

#### 随机化算法

> 之前对结课报告的要求理解有误，以为需要将课堂上讲过的所有算法用于0-1背包求解，于是借助随机化算法的思想，尝试着结合动态规划与贪心算法，按照蒙特卡罗方法模拟0-1背包问题。

##### 算法思想

> 1. 以贪心策略开始，得到一个较优的可行解$app$，根据贪心算法得到的解，将所有物品分为背包内和背包外两部分，分别记为$In$，$Out$；
> 2. 在$In$集内随机扔掉一件物品(i-th)，将i-th 放入Out集，则背包剩余空间$r$为$c-\sum\limits_{i\in In} w_i$，在Out集实施动态规划算法，得到在剩余空间r下的最优解$opt_r$；
> 3. 若$opt_r+\sum\limits_{i\in In} p_i \geq app$，则更新$app$并将$opt_r$中的物品从$Out$集放入$In$集；
> 4. 若$opt_r+\sum\limits_{i\in In} p_i < app$，则将i-th物品放回$In$集；
>
> 重复2-4过程，不断更新近似解，接近最优解。

```c++
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
```
##### 小结

> 对于蒙特卡罗方法，使用的并不优雅，生搬硬套，但也算本学期算法课程中的收获，于是记录在这里。
>
> 从贪心算法开始，时间复杂度是$O(nlog_2n)$，之后以动态规划算法在更小问题规模上迭代产生优化解，时间复杂度与空间复杂度都是$O(n^*c^*)$，大多数情况下，$c^* << c$，故而能大幅度降低时间复杂度与空间复杂度，而算法最终的解优于比贪心算法的近似解，应该也是可以接受的。

#### 归约算法

背包问题的规模可以通过以下过程降低：

+ 将物品集合N划分为三个部分，$J1,J0,F$

  + $J1=\{ j\in N: if x_j =1\quad in\; any\; opt\; of\; KP\}$
  + $J0=\{ j\in N: if x_j =0\quad in\; any\; opt\; of\; KP\}$
  + $F=N / \{J1 \or J0\}$

+ 于是背包问题可以归约为

  > $max \quad z=\sum\limits_{j\in F}p_jx_j + \hat p$
  >
  > $s.t. \quad \sum\limits_{j\in F} w_jx_j \leq \hat c$
  >
  > ​           $x_j = {0,1}, \; j\in F$ 
  >
  > 其中，$\hat p = \sum\limits_{j\in J1}p_j$，$\hat c=c-\sum\limits_{j\in J1}w_j$

##### Ingargiola-Korsh 算法

> 若令$x_j=b$产生了不可行解或比现有解更差的解，则在任意最优解里 都令$x_j=1-b$；
>
> 记$l$为KP问题的一个可行解，对$j \in N$，令$u_j^1$是在$x_j=1$条件下KP问题的上界，相对的，有$u_j^0$与$x_j=0$，则
>
> ​	$J1 = \{j \in N:\; u_j^0<l\}$
>
> ​	$J0 = \{j \in N:\; u_j^1<ld\}$
>
> 其中，$u_j^0$与$u_j^1$是使用Dantzig约束计算出的上界。

```c++
int IKR(vector<item> items, int n, int c, vector<int> &J0, vector<int> &J1){
  int s=0, w=0, l=0;
    for(int i=0; i<n; ++i) {
        w += items[i].weight;
        if(w>c) {	s=i;	break;}
        l += items[i].profit;
    }
    for(int j=0; j<s; ++j) {
        vector<item>::iterator iter = items.begin() + j;
        item tmp = *iter;
        iter = items.erase(iter);
        int uj0 = greedy(items, n-1, c);
        items.insert(--iter, tmp);
        if(uj0 < l) {
            J1.push_back(j);
        }
    }
    for(int j=s; j<n; ++j) {
        vector<item>::iterator iter = items.begin() + j;
        item tmp = *iter;
        iter = items.erase(iter);
        int uj1 = greedy(items, n-1, c-tmp.weight)+tmp.profit;
        items.insert(--iter, tmp);
        if(uj1 < l) {
            J0.push_back(j);
        }
    }
}
```
###参考文献

> 报告中并无直接引用，但是参考了很多以下文献的内容。

[1] Thomas H.Cormen, Charles E.Leiserson, Ronald L.Rivest,等. 算法导论(原书第3版)[J]. 计算机教育, 2013(4):110-110.

[2] Martello S, Toth P. Knapsack problems: algorithms and computer implementations[J]. Journal of the Operational Research Society, 1991, 42(6):513-513.

[3] Stephen Boyd, Lieven Vandenberghe, 王书. 凸优化[M]. 清华大学出版社, 2013.

[4] 张薇, 薛庆宇. 最优化方法[M]. 东北大学出版社, 2004.

[5] dinosoft. 蒙特卡罗树搜索+深度学习 -- AlphaGo原版论文阅读笔记. https://blog.csdn.net/dinosoft
