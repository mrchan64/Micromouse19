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
#include "../lib/Serial.hpp"
#include "SpeedSetter.hpp"
#include "WallState.hpp"
// #include "MazeSolver.hpp"
#include "AlgoState.hpp"

enum RunnerState{
  RUN_IDLE,
  RUN_IR_ALIGN_SIDE,
  RUN_IR_ALIGN_FRONT,
  RUN_STRAIGHT_CLOSED,
  RUN_STRAIGHT_LEFT_OPEN,
  RUN_STRAIGHT_RIGHT_OPEN,
  RUN_STRAIGHT_BOTH_OPEN,
  RUN_LEFT_TURN,
  RUN_RIGHT_TURN,
  RUN_U_TURN
};

const float DIST_CM_WALL                  = 3250.0f;
const float DIST_CM_TURN                  = 7600.0f;
const float DIST_TURN_CM                  = 4500.0f;
const float DIST_TO_ALIGN                 = 3000.0f;
const float DIST_FAST_ALIGN               = 1500.0f;
const float SEACH_SIDE_MARG               = 500.0f;
const float TIME_TO_ALIGN                 = 0.5f; //s
const float BEG_OFFSET_DIST               = 0000.0f;
const float NUDGE_DIST                    = 500.0f;
const float DIST_TURN_IMM                 = 3500.0f;
const float DIST_TO_CENTER_FROM_NEW       = 2000.0f;
const int CELLS_BEFORE_REALIGN            = 7;

void runStateRunner();


#endif