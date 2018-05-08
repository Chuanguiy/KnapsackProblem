#ifndef ITEM
#define ITEM
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
  void loadIn() {loaded=1;}
  void loadOut() {loaded=0;}
};
#endif
