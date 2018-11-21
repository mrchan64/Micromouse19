/************************************************************
 *                                                          *
 *   This FILE manages the Maze Solving of the Micromouse   *
 *                                                          *
 ************************************************************/

// AUTHOR: Matthew Chan

#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "mbed.h"
#include "MazeNode.hpp"
#include "IR_Detect.hpp"
#include "WallState.hpp"
#include "../lib/MiscIO.hpp"
#include "../lib/SensorComm.hpp"

// ENUMS -----------------------------------------------------
enum MazePhase {
  IDLE,
  ERROR,
  SEARCH,
  RESOLVE,
  RETURN,
  RUN
};

// CONSTANTS -------------------------------------------------
const Location TARGLOC{8,8};
const uint8_t MAZESIZE = 16;
const float START_IR_DIST = 1.0f;

// Objects ---------------------------------------------------
extern Node[16][16] currMaze;
extern Location currLoc;

// METHODS ---------------------------------------------------
void initialize_maze();
void populate_wall(Node cell, Direction dir, WallState state);
void bfs(Node start, Node end);
void vfield(Node loc);

void run_maze_solve_cycle();

#endif