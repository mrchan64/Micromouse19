/**************************************************************
 *                                                            *
 *   This FILE manages the detected walls of the Micromouse   *
 *                                                            *
 **************************************************************/

// AUTHOR: Matthew Chan

#ifndef WALLSTATE_H
#define WALLSTATE_H

#include "mbed.h"
#include "IR_Detect.hpp"

// ENUMS -----------------------------------------------------
struct CellWalls {
  bool left;
  bool right;
  bool front;
};

struct WallSlopes {
  LinReg left;
  LinReg right;
}

// CONSTANTS -------------------------------------------------
// Note that the window is 100 points right now, so slope of .3/100 = .003 is pretty reasonable rise
// MAKE SURE TO REFER TO IR_Detect.hpp TO SEE HOW IR NORMALIZATION WORKS
/* STEADY WALL PROFILE */
const float WALL_ABSBMAX              = 0.001;
const float WALL_AMAX                 = 0.5;
const float WALL_R2MIN                = 0.8;

/* WALL START PROFILE */
const float START_BMAX                = -.002;
const float START_AMIN                = 0.7;
const float START_R2MIN               = 0.5;

/* WALL END PROFILE */
const float END_BMIN                  = .002;
const float END_AMAX                  = 0.3;
const float END_R2MIN                 = 0.5;

// Variables -------------------------------------------------
extern CellWalls currWalls;
extern CellWalls wallStateChange;
extern WallSlopes currSlopes;
extern bool can_check_wall;

// METHODS ---------------------------------------------------
CellWalls get_curr_walls();
void run_wall_update_cycle();
void reset_walls();

#endif