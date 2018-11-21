/***************************************************************
 *                                                             *
 *   This FILE sets up the IR sensors used in the Micromouse   *
 *                                                             *
 ***************************************************************/

// AUTHOR: Matthew Chan

#ifndef INFRARED_H
#define INFRARED_H

#include "mbed.h"
#include "PinOut.hpp"

// CONSTANTS -------------------------------------------------
const int LED_ON_US           = 50;
const int LED_OFF_US          = 10;

// DATA STRUCTS ----------------------------------------------
struct IRData {
  float l;
  float r;
  float lf;
  float rf;
};

// ENUMS -----------------------------------------------------
enum AsyncIRState {
  IROFF,
  IDLE1,
  SCANFORWARD,
  SCANSIDE,
  IDLE2
};

// METHODS ---------------------------------------------------
void initialize_IR();
void read_forward(IRData* ird);
void read_side(IRData* ird);
IRData read_all();

// ASYNC -----------------------------------------------------
bool cycle_IR_async();
IRData read_IR_async();
void off_IR_async();
void on_IR_async();

#endif