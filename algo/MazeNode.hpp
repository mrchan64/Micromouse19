/********************************************************************
 *                                                                  *
 *   This FILE manages the individual node data of the Micromouse   *
 *                                                                  *
 ********************************************************************/

// AUTHOR: Matthew Chan

#ifndef NODE_H
#define NODE_H

#include "mbed.h"

// STRUCTS ---------------------------------------------------
struct Location {
  uint8_t x;
  uint8_t y;
};

// ENUMS -----------------------------------------------------
enum WallState {
  UNSURE,
  OPEN,
  WALL
};

enum Direction {
  NORTH,
  WEST,
  SOUTH,
  EAST,
  NORTHWEST,
  SOUTHWEST,
  SOUTHEAST,
  NORTHEAST,
  NONE
};

enum Visited {
  UNVISITED,
  VISITED
};

enum BFSExplored {
  UNEXPLORED,
  EXPLORED
};

// CLASSES ---------------------------------------------------
class Node{
public:
  Node();
  Node(int x_pos, int y_pos);
  void setXY(int x_pos, int y_pos);
  WallState nbwalls[4];
  Node* nb[4];
  Node* previous;
  Visited visited;
  bool toVisit;
  BFSExplored bfsexplored;
  Location loc;
  int bfsScore;
  void resolveWall(int dir, bool hasWall);
  void visit();
  Direction entrance;
  Direction exit;
  Direction angle;
};

// METHODS ---------------------------------------------------
Direction intToDir(int dir);

#endif