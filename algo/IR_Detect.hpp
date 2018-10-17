/************************************************************
 *                                                          *
 *   This FILE manages IR normalization in the Micromouse   *
 *                                                          *
 ************************************************************/

// AUTHOR: Matthew Chan

#ifndef IR_DETECT_H
#define IR_DETECT_H

#include "mbed.h"
#include "../lib/Infrared.hpp"

// CONSTANTS -------------------------------------------------
const int REVIEW_WINDOW       = 100;

// STRUCTS ---------------------------------------------------
struct LinReg{
  float a;
  float b;
  float r;
};

// METHODS ---------------------------------------------------
void normalize_sensors();
void run_IR_cycle();
void detect_wall();
int read_side();
IRData get_current_data();
IRData get_curr_norm_data();

#endif