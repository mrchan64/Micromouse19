/**************************************************************
 *                                                            *
 *   This FILE manages the CellNavigation of the Micromouse   *
 *                                                            *
 **************************************************************/

// AUTHOR: Matthew Chan

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "mbed.h"
#include "IR_Detect.hpp"
#include "MazeNode.hpp"
#include "WallState.hpp"
#include "../lib/Motors.hpp"

// ENUMS -----------------------------------------------------
// Only Straight and Curve 90 should be used on exploration rounds
enum CellType {
  STRAIGHT,
  CURVE_90,
  U_TURN,
  CURVE_45,
  STRAIGHT_45
};

enum MotorSpeed {
  float left;
  float right;
}

// CONSTANTS -------------------------------------------------
/* NOTE THAT IM GONNA USE 9 CM IS WHERE WHEELS ARE AT MIDDLE */
/* STRAIGHT CONSTANTS */
/* WANT TO KEEP DISTANCE AT 0 AND SLOPE AT 0*/ // test this stuff first for straight line
const float BTOSTEP               = -0.1f;
const float ATOSTEP               = -2.0f;
const float SCAN_WALL_ZONE        = 6.0f; // For checking if wall starts // Need to fix this measurement
const float SCAN_WALL_ZONE_END    = 9.0f;
const float STRAIGHT_END          = 18.0f;

const float SCAN_WALL_START_ALIGN = 7.0f; // DO MATH
const float SCAN_WALL_END_ALIGN   = 8.0f;// DO MATH

/* CURVE_90 CONSTANTS */
/* WE CAN SCAN FOR WALLS IN PART 1 (front) AND PART 2 */
const float DIST_90_1             = (CELL_DIST - TURN_DIAM/2.0f) / 2.0f;
const float ANGLE_90_1            = 90.0f;
const float DIST_90_2             = (CELL_DIST - TURN_DIAM/2.0f) / 2.0f;

/* CURVE_45 CONSTANTS */
/* WE DONT DO SCANNING HERE UNLESS WE ARE EXPECTING WALLS, FIGURE OUT THAT LOGIC LATER*/
const float ANGLE_45_1            = 90.0f;
const float DIST_45_C_1           = 12.726f;

/* STRAIGHT_45 CONSTANTS */
const float DIST_45_S_1           = 12.726f;
const float ENC_DEL_TO_STEP       = 10;

/* U_TURN CONSTANTS */
const float DIST_U              = (CELL_DIST) / 2.0f;
const float ANGLE_U               = 90.0f;

// Variables -------------------------------------------------
extern CellType currCellType;
extern MotorSpeed curr_motor_speed;
extern int abs_orient;
extern Node[16][16] currMaze;
extern Location currLoc;
extern float cellStraightDist;
extern bool cellFinished;

// METHODS ---------------------------------------------------
void reset_nav();
void new_cell();
void run_cell_nav_cycle();

#endif