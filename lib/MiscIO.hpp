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
const float V_MULT                  = 10.4151f;
const float V_MIN                   = 7.0f;

// STRUCTS ---------------------------------------------------
class LEDS{
public:
  LEDS(int a);
  void off();
  void on(int a, int b, int c, int d, int e);
  DigitalOut l1;
  DigitalOut l2;
  DigitalOut l3;
  DigitalOut l4;
  DigitalOut l5;
};

extern LEDS led;
// extern AnalogIn voltmeter;

void cycle_leds(float time);
float get_voltage();

#endif