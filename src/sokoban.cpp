#include "util.h"
#include "sokoban.h"
#include "bfsearch.h"
#include <iostream>

using namespace std;


void SokobanMaker::prepareBoulderRooms(Rectangle area, Range mainWidth, Range otherWidth) {
  Vec2 mainSize(random.get(mainWidth), random.get(mainWidth));
  Vec2 mainPos((area.width() - mainSize.x) / 2, (area.height() - mainSize.y) / 2);
  Rectangle mainRect(mainPos, mainPos + mainSize);
  for (Vec2 v : mainRect)
    level[v] = '.';
  vector<int> roomSides;
  for (int i : Range(4))
    roomSides.push_back(i);
  roomSides = random.permutation(roomSides);
  for (int i = 0; i < numRooms - 1; ++i) {
    Vec2 size;
    do {
      size = Vec2(random.get(otherWidth), random.get(otherWidth));
    } while (size.x <= 2 && size.y <= 2);
    Vec2 pos;
    bool door = (i < numDoors);
    switch (roomSides[i % 4]) {
      case 0:
        pos = Vec2(mainRect.right(), random.get(
                     max(area.top(), mainRect.top() - size.y + 1),
                     min(area.bottom(), mainRect.bottom())));
        if (door) {
          pos += Vec2(1, 0);
          level[Vec2(mainRect.right(), random.get(max(mainRect.top(), pos.y), min(mainRect.bottom(), pos.y + size.y)))] = '.';
        }
        break;
      case 1:
        pos = Vec2(random.get(
                     max(area.left(), mainRect.left() - size.x + 1),
                     min(area.right(), mainRect.right())), mainRect.bottom());
        if (door) {
          pos += Vec2(0, 1);
          level[Vec2(random.get(max(mainRect.left(), pos.x), min(mainRect.right(), pos.x + size.x)), mainRect.bottom())] = '.';
        }
        break;
      case 2:
        pos = Vec2(mainRect.left() - size.x, random.get(
                     max(area.top(), mainRect.top() - size.y + 1),
                     min(area.bottom(), mainRect.bottom())));
        if (door) {
          pos += Vec2(-1, 0);
          level[Vec2(mainRect.left() - 1, random.get(max(mainRect.top(), pos.y), min(mainRect.bottom(), pos.y + size.y)))] = '.';
        }
        break;
      case 3:
        pos = Vec2(random.get(
                     max(area.left(), mainRect.left() - size.x + 1),
                     min(area.right(), mainRect.right())), mainRect.top() - size.y);
        if (door) {
          pos += Vec2(0, -1);
          level[Vec2(random.get(max(mainRect.left(), pos.x), min(mainRect.right(), pos.x + size.x)), mainRect.top() - 1)] = '.';
        }
        break;
    }
    for (Vec2 v : Rectangle(pos, pos + size))
      level[v] = '.';
  }
}

SokobanMaker::SokobanMaker(RandomGen& r, Vec2 levelSize, int boulders, int nodes)
  : random(r), level(levelSize, '#'), bestLevel(levelSize, '?'), numNodes(nodes), numBoulders(boulders),
    distanceTable(Rectangle(levelSize)) {
}


SokobanMaker&SokobanMaker::setNumRooms(int r) {
  numRooms = r;
  return *this;
}

SokobanMaker&SokobanMaker::setNumDoors(int d) {
  numDoors = d;
  return *this;
}

static void printLevel(const Table<char>& level) {
  for (int y : level.getBounds().getYRange()) {
    for (int x : level.getBounds().getXRange())
      cout << level[Vec2(x, y)];
    cout << '\n';
  }
}

bool SokobanMaker::make() {
  Rectangle area(level.getBounds());
  for (Vec2 v : area)
    level[v] = '#';
  int prizeRoomRadius = 1;
  int boulderRoomWidth = level.getBounds().width() - 1 - 2 * prizeRoomRadius - 1 - numBoulders;
  prepareBoulderRooms(Rectangle(area.topLeft(), Vec2(boulderRoomWidth, area.height())), Range(3, 5), Range(2, 4));
  //printLevel(level);
  Vec2 start;
  for (int x : area.getXRange().shorten(prizeRoomRadius).reverse())
    for (int y : random.permutation(area.getYRange().shorten(prizeRoomRadius)))
      if (level[Vec2(x, y)] == '.') {
      start = Vec2(x, y);
      goto found;
    }
  found:
  middleLine = start.x;
  workArea = Rectangle(area.topLeft(), Vec2(start.x + numBoulders, area.bottom()));
  for (int i : Range(1, numBoulders + 1)) {
    Vec2 pos = start + Vec2(i, 0);
    level[pos] = '0';
    boulders.push_back(pos);
  }
  level[start + Vec2(numBoulders + 1, 0)] = '+';
  for (Vec2 v : Rectangle::centered(start + Vec2(numBoulders + prizeRoomRadius + 2, 0), prizeRoomRadius))
    level[v] = '.';
  set<int> visited;
  Vec2 curPos = start;
  moveBoulder(0, curPos, visited);
  for (int i : Range(1, numBoulders + 1)) {
    Vec2 holePos = start + Vec2(i, 0);
    if (holePos == finalPos || bestLevel[holePos] != '.')
      return false;
    bestLevel[holePos] = '^';
  }
  bestLevel[finalPos] = '@';
  return true;
}

Table<char> SokobanMaker::getResult() {
  return bestLevel;
}

int SokobanMaker::getMaxDepth() {
  return maxDepth;
}

int SokobanMaker::getHash(const vector<Vec2>& boulders, Vec2 curPos) {
  int seed = 0;
  for (auto& i : boulders) {
    seed ^= i.x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= i.y + 0x9e3779c9 + (seed << 6) + (seed >> 2);
  }
  return seed;
}

bool SokobanMaker::isFree(Vec2 pos) {
  return pos.inRectangle(workArea) && level[pos] == '.';
}

void SokobanMaker::moveBoulder(int depth, Vec2& curPos, set<int>& visited) {
  if (depth > maxDepth) {
    bestLevel = level;
    maxDepth = depth;
    finalPos = curPos;
  }
  if (visited.size() > numNodes)
    return;
  BfSearch bfSearch(distanceTable, workArea, curPos, [&](Vec2 pos) { return isFree(pos);}, Vec2::directions4());
  for (Vec2 boulderPos : random.permutation(boulders)) {
    for (Vec2 v : Vec2::directions4(random)) {
      if (!bfSearch.isReachable(boulderPos + v) || (boulderPos.x >= middleLine - 1 && v.x > 0))
        continue;
      Vec2 pos = boulderPos + v;
      Vec2 dest(-100, -1);
      for (int i = 1; isFree(pos + v * i) && (v.x <= 0 || pos.x + v.x * i <= middleLine); ++i)
        dest = pos + v * random.get(1, i + 1);
      if (dest.x == -100)
        continue;
      CHECK(level[dest] == '.');
      CHECK(level[pos - v] == '0');
      int boulderIndex = findElement(boulders, pos - v);
      boulders[boulderIndex] = dest - v;
      level[dest - v] = '0';
      level[pos - v] = '.';
      Vec2 prevPos = curPos;
      curPos = dest;
      int hash = getHash(boulders, curPos);
      if (!visited.count(hash)) {
        visited.insert(hash);
        moveBoulder(depth + 1, curPos, visited);
      }
      CHECK(level[pos - v] == '.');
      CHECK(level[dest - v] == '0');
      level[dest - v] = '.';
      level[pos - v] = '0';
      boulders[boulderIndex] = pos - v;
      curPos = prevPos;
    }
  }
}
