#include <iostream>
#include <thread>
#include "util.h"
#include "sokoban.h"
#include "cxxopts.h"

using namespace std;

void printLevel(const Table<char>& level) {
  for (int y : level.getBounds().getYRange()) {
    for (int x : level.getBounds().getXRange())
      cout << level[Vec2(x, y)];
    cout << '\n';
  }
}

void trySokoban(RandomGen& randomGen, Vec2 levelSize, int numTries,
                int numBoulders, int numMoves, int rooms) {
  int maxDepth = -1;
  for (int i = 0; i < numTries; ++i) {
    SokobanMaker sokoban(randomGen, levelSize, numBoulders, numMoves);
    sokoban.setNumRooms(rooms);
    if (sokoban.make() && sokoban.getMaxDepth() > maxDepth) {
      maxDepth = sokoban.getMaxDepth();
      cout << "Depth reached: " << maxDepth << endl;
      printLevel(sokoban.getResult());
    }
  }
  if (maxDepth == -1)
    cout << "Unable to generate a level with these parameters" << endl;
}

int main(int argc, char* argv[]) {
  cxxopts::Options options("Sokoban generator", "Generates sokoban levels.");
  options.add_options()
    ("h,help", "Display help")
    ("t,tries", "Number of tries per thread", cxxopts::value<int>()->default_value("1000"))
    ("x,width", "Width of level", cxxopts::value<int>()->default_value("30"))
    ("y,height", "Height of level", cxxopts::value<int>()->default_value("20"))
    ("r,rooms", "Number of rooms in the puzzle", cxxopts::value<int>()->default_value("3"))
    ("b,boulders", "Number of boulders", cxxopts::value<int>()->default_value("6"))
    ("m,moves", "Number of boulder moves in search", cxxopts::value<int>()->default_value("500"))
      ;
  options.parse(argc, argv);
  if (options.count("help")) {
    cout << options.help() << endl;
    return 0;
  }
  int tries = options["tries"].as<int>();
  Vec2 levelSize(options["width"].as<int>(), options["height"].as<int>());
  int boulders = options["boulders"].as<int>();
  int moves = options["moves"].as<int>();
  int rooms = options["rooms"].as<int>();
  RandomGen randomGen;
  randomGen.init(time(0));
  trySokoban(randomGen, levelSize, tries, boulders, moves, rooms);
}
