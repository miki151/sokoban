#pragma once

#include <functional>
#include "util.h"

class DistanceTable {
  public:
  DistanceTable(Rectangle bounds);

  double getDistance(Vec2 v) const;
  void setDistance(Vec2 v, double d);
  void clear();

  private:
  Table<double> ddist;
  Table<int> dirty;
  int counter = 1;
};



class BfSearch {
  public:
  BfSearch(DistanceTable&, Rectangle bounds, Vec2 from, function<bool(Vec2)> entryFun, vector<Vec2> directions = Vec2::directions8());
  bool isReachable(Vec2) const;
  const set<Vec2>& getAllReachable() const;

  private:
  set<Vec2> reachable;
  DistanceTable& table;
};

