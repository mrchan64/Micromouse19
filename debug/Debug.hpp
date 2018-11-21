/******************************************************************
 *                                                                *
 *   This FILE sets up the Debug Methods used in the Micromouse   *
 *                                                                *
 ******************************************************************/

// AUTHOR: Matthew Chan

#ifndef M_DEBUG_H
#define M_DEBUG_H

#include "mbed.h"
#include "../lib/Motors.hpp"
#include "../lib/SensorComm.hpp"
#include "../lib/Serial.hpp"
#include "../algo/IR_Detect.hpp"

// METHODS ---------------------------------------------------
void print_accel_output();
void test_align_front();

#endif