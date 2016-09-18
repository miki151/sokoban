#include "bfsearch.h"
#include <queue>

const static double infinity = 1000000000;

BfSearch::BfSearch(DistanceTable& t, Rectangle bounds, Vec2 from, function<bool(Vec2)> entryFun, vector<Vec2> directions) : table(t) {
  table.clear();
  queue<Vec2> q;
  table.setDistance(from, 0);
  q.push(from);
  int numPopped = 0;
  while (!q.empty()) {
    ++numPopped;
    Vec2 pos = q.front();
    q.pop();
    CHECK(!reachable.count(pos));
    reachable.insert(pos);
    for (Vec2 dir : directions) {
      Vec2 next = pos + dir;
      if (next.inRectangle(bounds) && table.getDistance(next) == infinity && entryFun(next)) {
        table.setDistance(next, 0);
        q.push(next);
      }
    }
  }
}

bool BfSearch::isReachable(Vec2 pos) const {
  return reachable.count(pos);
}

const set<Vec2>& BfSearch::getAllReachable() const {
  return reachable;
}

DistanceTable::DistanceTable(Rectangle bounds) : ddist(bounds), dirty(bounds, 0) {}

double DistanceTable::getDistance(Vec2 v) const {
  return dirty[v] < counter ? infinity : ddist[v];
}

void DistanceTable::setDistance(Vec2 v, double d) {
  ddist[v] = d;
  dirty[v] = counter;
}

void DistanceTable::clear() {
  ++counter;
}
