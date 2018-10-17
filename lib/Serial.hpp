/***********************************************************
 *                                                         *
 *   This FILE sets up the Serial used in the Micromouse   *
 *                                                         *
 ***********************************************************/

// AUTHOR: Matthew Chan

#ifndef M_SERIAL_H
#define M_SERIAL_H

#include "mbed.h"
#include "PinOut.hpp"

// CONSTANTS -------------------------------------------------
const int BAUD            = 115200;

// OBJECTS ---------------------------------------------------
extern Serial pc;

// METHODS ---------------------------------------------------
void print(char* str);
void println(char* str);

#endif