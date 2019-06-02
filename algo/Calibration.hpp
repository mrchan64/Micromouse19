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
const float REST_TIME_US = 4;

//    lf           | l             | r             | rf
// S: -1.0000      | -19.8305      | -23.2670      | -1.0000       |
// F: 0.2392       | 0.9065        | 0.8409        | 0.0631        |
// B: 0.5497       | 0.4757        | 0.7580        | 0.5182        |
const IRData PREV_S = {-1.0f, -5.3996f, -9.009f, -1.0f};
const IRData PREV_B = {0.4632f, 0.6016f, 0.8285f, 0.7930f};
const IRData PREV_F = {0.1862f, 1.0f, 1.0f, 0.1468f};

// const IRData PREV_F2 = {.0248f, 0.0f, 0.0f, 0.0f};
// const IRData PREV_B2 = {.3910f, 0.0f, 0.0f, 0.0f};


//    lf           | l             | r             | rf
// S: -1.0000      | -11.3122      | -16.9917      | -1.0000       |
// F: 0.0512       | 0.4033        | 2.8332        | 0.0760        |
// B: 0.4740       | 0.7668        | 0.4567        | 0.5379        |




// METHODS ---------------------------------------------------
void pre_code();
void mid_code();
void post_code();
void move_dist(float dist_cm);
void move_ticks(float dist_ticks);
void run_calibration();
void align_front(float dist);

void run_threshold_calibration();
void set_threshold_manual();

#endif