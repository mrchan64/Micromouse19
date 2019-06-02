/***********************************************************************
 *                                                                     *
 *   This FILE sets up the LEDs and Voltmeter used in the Micromouse   *
 *                                                                     *
 ***********************************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "mbed.h"
#include "MiscIO.hpp"

LEDS::LEDS(int a): l1(LED_1), l2(LED_2), l3(LED_3), l4(LED_4), l5(LED_5) {}

void LEDS::off(){
  led.l1 = 0;
  led.l2 = 0;
  led.l3 = 0;
  led.l4 = 0;
  led.l5 = 0;
}

void LEDS::on(int a, int b, int c, int d, int e){
  led.l1 = a;
  led.l2 = b;
  led.l3 = c;
  led.l4 = d;
  led.l5 = e;
}

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
}

float get_voltage(){
  return V_MULT * voltmeter.read();
}