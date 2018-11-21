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
const int REVIEW_WINDOW           = 100;
const float THRESHOLD_MULTIPLIER  = 0.8f;

// STRUCTS ---------------------------------------------------
struct LinReg{
  float a;
  float b;
  float r;
};

// OBJECTS ---------------------------------------------------
extern IRData window[REVIEW_WINDOW];
extern IRData normwindow[REVIEW_WINDOW];
extern IRData norm_total;

/* Documentation on how f_thresh is calibrated:
 * .l is left which is left sensor pointing forward
 * .r is right which is right sensor pointing forward
 * .lf is left-front which is forward sensor pointing left
 * .rf is right-front which is forward sensor pointing right
 *
 * THESE VALUES ARE POST SCALING
 *
 * left/right: (forward)
 *    - 0 denotes wall dist at calibration time
 *    - (-1) denotes wall dist when moving forward during calibration
 *    - (-) <- (wall detected) < f_thresh.l/.r < (wall not detected) < (+)
 * left/right-front (sides)
 *    - 0 denotes wall dist at calibration time (wall shown)
 *    - (-) <- (wall detected) < f_thresh.lf/rf < (wall not detected) < (+)
 */
extern IRData f_thresh;

// METHODS ---------------------------------------------------
LinReg linear_regression(float* data, float ymean);
void normalize_IR_sensors();
void scale_IR_sensors();
void set_IR_threshold_forward();
void set_IR_threshold_left();
void set_IR_threshold_right();
void run_IR_cycle();
void detect_wall();
IRData get_current_IR_data();
IRData get_curr_norm_IR_data();

#endif