/***********************************************************************
 *                                                                     *
 *   This FILE sets up the LEDs and Voltmeter used in the Micromouse   *
 *                                                                     *
 ***********************************************************************/

// AUTHOR: Matthew Chan

#include "mbed.h"
#include "PinOut.hpp"

#ifndef MISCIO_H
#define MISCIO_H

// CONSTANTS -------------------------------------------------
const float LIPO_MAX                = 8.4f;
const float LIPO_MIN                = 7.0f;

// STRUCTS ---------------------------------------------------
class LEDS{
public:
  LEDS(int a);
  DigitalOut l1;
  DigitalOut l2;
  DigitalOut l3;
  DigitalOut l4;
  DigitalOut l5;
  DigitalOut l6;
};

extern LEDS led;
extern AnalogIn voltmeter;

void cycle_leds(float time);

#endif