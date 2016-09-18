# Sokoban
This is a random sokoban level generator. It doesn't have any external dependencies. You'll need a c++11 compiler, such as gcc or clang, to compile it.

## Algorithm

Initially, boulders are placed at their destinations, and the algorithm pulls them in random directions to simulate the game in reverse.
Backtracking is used when the algorithm gets stuck.<br><br>
After a set number of positions is analyzed, the algorithm returns the most distant solution from the initial configuration, counting the number of moves.
For the lack of a good evaluating function, we assume the number of moves it took to reach a position is correlated with its difficulty. Note: solving a position almost always takes much fewer moves than the algorithm took to generate it.<br><br>
The program runs many iterations and prints the solution of the greatest assumed difficulty after it's finished.

Each level consists of a fixed "prize" room, a corridor of holes, where the boulders are to be moved by the player, and a group of connected rooms containing boulders, and the player's starting position. The number of rooms and their positioning can be tweaked via command line.

```
############################   # - wall
######....##################   . - floor
######..00##################   @ - player's starting position
######..0.########...#######   0 - boulder
######..0.^^^^^^^+...#######   ^ - hole
#######0##########...#######   + - door to the prize room
#######@..##################
#######00.##################
#######...##################
############################
```
## Usage

To compile and run, simply enter:
```
make
./sokoban
```
```
Usage:
  Sokoban generator [OPTION...]

  -h, --help            Display help
  -t, --iterations arg  Number of iterations (default: 1000)
  -x, --width arg       Width of level (default: 28)
  -y, --height arg      Height of level (default: 16)
  -r, --rooms arg       Number of rooms in the puzzle (default: 3)
  -d, --doors arg       Number of rooms that are attached via door (default: 1)
  -b, --boulders arg    Number of boulders
  -p, --positions arg   Number of positions analyzed in each search (default: 500)

```
