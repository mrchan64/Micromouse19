/***********************************************************
 *                                                         *
 *   This FILE sets up the motors used in the Micromouse   *
 *                                                         *
 ***********************************************************/

// AUTHOR: Matthew Chan

#ifndef MOTORS_H
#define MOTORS_H

#include "mbed.h"
#include "PinOut.hpp"
#include "encoder.hpp"

// CONSTANTS -------------------------------------------------
const int PERIOD_US             = 50;
const float MIN_SPEED           = 0.079f;
const float MAX_SPEED           = 0.7f;
const float MAX_SPEED_SOFT      = 0.4f;
const float SPEED_STEP          = 0.02f;
const float RAMP_SPEED          = 0.00000124f;  // del(power)/usecond, want to go .079 -> .7 in .5 second

const float ONE_REV             = 617.0f;
const float WHEEL_DIAM          = 4.058f;
const float CELL_DIST           = 18.0f;         // cm; need to calibrate
const float TURN_DIAM           = 14.39f;       // 14.68 rot when attached? motor slip?

const float PI                  = 3.1415925634f;

// MOTOR CLASS -----------------------------------------------
class Motor{
public:
  Motor(char side);
  void setSpeed(float speed);
  float getSpeed();
  float stepUp();
  float stepDown();
  void setStepAboveSpeed(float speed, int step);
  void moveDistance(float dist_cm);
  void moveTicks(float dist_ticks);
  void resetEncoder();
  int getTicks();
  float getDist();
private:
  PwmOut channel_a;
  PwmOut channel_b;
  Encoder enc;
  float curr_speed;
};

// MORE OBJECTS ----------------------------------------------
extern Motor m_left;
extern Motor m_right;

#endif