/************************************************************
 *                                                          *
 *   This FILE manages the Maze Solving of the Micromouse   *
 *                                                          *
 ************************************************************/

// AUTHOR: Matthew Chan

#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include "mbed.h"
#include "MazeNode.hpp"
#include "IR_Detect.hpp"
#include "WallState.hpp"
#include "../lib/MiscIO.hpp"
#include "../lib/SensorComm.hpp"

// ENUMS -----------------------------------------------------
enum MazePhase {
  IDLE,
  MAZEERROR,
  SEARCH,
  RESOLVE,
  RETURN,
  RUN
};

// CONSTANTS -------------------------------------------------
const Location TARGLOC = {8,8};
const uint8_t MAZESIZE = 16;
const int ARRLEN = 256;
const float START_IR_DIST = 1.0f;

// Objects ---------------------------------------------------
extern Node currMaze[16][16];
extern Location currLoc;

// METHODS ---------------------------------------------------
void initialize_maze();
void populate_wall(Node cell, Direction dir, WallState state);
void bfs(Node start, Node end);
// void vfield(Node loc);
void visitSquare(Direction dir, CellWalls cw, bool wallOnLeft, bool wallOnRight);
Node* nextToMidSquare();
int getDir();

Direction runRight(Direction dir, CellWalls cw);


// void run_maze_solve_cycle();

#endif