/***********************************************************************
 *                                                                     *
 *   This FILE sets up the LEDs and Voltmeter used in the Micromouse   *
 *                                                                     *
 ***********************************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "mbed.h"
#include "MiscIO.hpp"

LEDS::LEDS(int a): l1(LED_1), l2(LED_2), l3(LED_3), l4(LED_4), l5(LED_5),  l6(LED_6) {}

LEDS led(1);
AnalogIn voltmeter(VOLTMETER);

void cycle_leds(float time) {
  led.l1 = 1;
  wait(time);
  led.l1 = 0;
  led.l2 = 1;
  wait(time);
  led.l2 = 0;
  led.l3 = 1;
  wait(time);
  led.l3 = 0;
  led.l4 = 1;
  wait(time);
  led.l4 = 0;
  led.l5 = 1;
  wait(time);
  led.l5 = 0;
  led.l6 = 1;
  wait(time);
  led.l6 = 0;
}