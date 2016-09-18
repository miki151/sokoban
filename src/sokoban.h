#pragma once

#include "util.h"
#include "bfsearch.h"

class SokobanMaker {
  public:
  SokobanMaker(RandomGen& random, Vec2 levelSize, int numBoulders, int numNodes);

  SokobanMaker& setNumRooms(int);
  SokobanMaker& setNumDoors(int);

  bool make();
  Table<char> getResult();
  int getMaxDepth();

  private:
  void prepareBoulderRooms(Rectangle area, Range mainWidth, Range otherWidth);
  int middleLine;
  Rectangle workArea = Rectangle(1, 1);
  vector<Vec2> boulders;
  RandomGen& random;
  Table<char> level;
  Table<char> bestLevel;
  Vec2 finalPos;
  int maxDepth = 1;
  void moveBoulder(int depth, Vec2& curPos, set<int>& visited);
  bool isFree(Vec2 pos);
  int getHash(const vector<Vec2>& boulders, Vec2 curPos);
  int numNodes;
  int numBoulders;
  DistanceTable distanceTable;
  int numRooms = 3;
  int numDoors = 12345;
};

