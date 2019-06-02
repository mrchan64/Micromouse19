/*******************************************************
 *                                                     *
 *   This FILE manages Motor Speed in the Micromouse   *
 *                                                     *
 *******************************************************/

// AUTHOR: Matthew Chan

#ifndef SPEED_SETTER_H
#define SPEED_SETTER_H

#include "mbed.h"
#include "../lib/Motors.hpp"
#include "../lib/MiscIO.hpp"
#include "WallState.hpp"

// ENUMS -----------------------------------------------------
enum PIDState {
  STRAIGHT,
  LEFT,
  RIGHT,
  UTURN
};

// CONSTANTS -------------------------------------------------
// const float MAX_TPS                 = 4000.0f;
// const float TPS_K                   = 0.000005f;
const float CONST_DIST              = 2000.0f; // where to start pid from
const float CONST_SPEED             = 10000.0f;
// const float CONST_SPEED             = 10000.0f;
// const float RAMP_DOWN_DIST          = 20000.0f; // ticks
const float UNIT_STEP_RANGE         = 50.0f;
const float CORRECTION_BOOST        = 0.12f;
const float BOOST_RANGE             = 500.0f;
// const float RAMP_SPEED              = .000123f;
const int TICK_COMPLETION_MARGIN    = 4;
const float PROP_K                  = 1.0f;
const float INTEG_K                 = 0.25f;
const float DERIV_K                 = 450.0f;
// const float PROP_K                  = 0.0f;
// const float INTEG_K                 = 0.0f;
// const float DERIV_K                 = 0.0f;
const float SPEED_MULT              = 0.00008f;

// speed PID
const float SPID_PROP_K             = 0.0f;
const float SPID_INTEG_G            = .009f;
const float SPID_DERIV_K            = 0.0f;
const float SPID_MULT               = 0.00006f;

const int NUM_DERIV_TIMES           = 100;

const float ONE_CELL_TICKS          = 7250.0f; 
const float RIGHT_ANG_TICKS         = 3850.0f;
const float PAST_OPENING_TICKS      = 1000.0f;

// turning PID with gyro
// const float DOM_PROP_K              = 1.0f;
// const float DOM_INTEG_K             = 0.12f;
// const float DOM_DERIV_K             = 450.0f;
const float TURN_SPEED_MULT         = .00001f;

const float IR_STRAIGHT_MULT        = 4.0f;
const float WALL_DIFF_DERIV_MULT    = 64.0f;
const int IR_CORR_STEP = 30;

// OBJECTS ---------------------------------------------------

// METHODS ---------------------------------------------------
void setDistance(float distance);
void updateDistance(float distance);
void initSpeedSetter();
void setDistanceTicks(float ticks);
void setAngleDegrees(PIDState dir);
void setSpeedTPS(float tps);
void resetSpeedSetter();
void runSpeedSetter();
bool getSpeedCompletion();
float getCurrDist();
void toggleIRRun(bool on);

#endif