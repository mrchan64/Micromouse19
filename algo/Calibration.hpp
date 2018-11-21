/********************************************************************
 *                                                                  *
 *   This FILE manages the starting calibration of the Micromouse   *
 *                                                                  *
 ********************************************************************/

// AUTHOR: Matthew Chan

#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "mbed.h"
#include "../lib/Infrared.hpp"
#include "../lib/MiscIO.hpp"
#include "../lib/Motors.hpp"
#include "../lib/Serial.hpp"
#include "../lib/SensorComm.hpp"
#include "IR_Detect.hpp"

// CONSTANTS -------------------------------------------------
const float REST_TIME_US = .5;

// METHODS ---------------------------------------------------
void pre_code();
void mid_code();
void post_code();
void turn_90();
void move_dist(float dist_cm);
void move_ticks(float dist_ticks);
void run_calibration();
void align_front(float dist);

#endif