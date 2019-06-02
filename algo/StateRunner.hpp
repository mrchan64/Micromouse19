/*******************************************************
 *                                                     *
 *   This FILE manages Cell State in the Micromouse    *
 *                                                     *
 *******************************************************/

// AUTHOR: Matthew Chan

#ifndef STATE_RUNNER_H
#define STATE_RUNNER_H

#include "mbed.h"
#include "../lib/MiscIO.hpp"
#include "SpeedSetter.hpp"
#include "WallState.hpp"
#include "MazeSolver.hpp"

enum RunnerState{
  IDLE,
  RUN_IR_ALIGN_SIDE,
  RUN_IR_ALIGN_FRONT,
  RUN_STRAIGHT_CLOSED,
  RUN_STRAIGHT_LEFT_OPEN,
  RUN_STRAIGHT_RIGHT_OPEN,
  RUN_STRAIGHT_BOTH_OPEN,
  RUN_LEFT_TURN,
  RUN_RIGHT_TURN
};

const float END_OF_CELL_TO_TURN           = 4900.0f;
const float DIST_TO_ALIGN                 = 5000.0f;
const float TIME_TO_ALIGN                 = 0.5f; //s
const float END_OF_CELL_ALIGN_DIST        = ???
const float BEG_OF_CELL_ALIGN_DIST        = ???
const float BEG_OFFSET_DIST               = 0.0f;
const int CELLS_BEFORE_REALIGN            = 7;

void runStateRunner();


#endif