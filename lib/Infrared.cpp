/***************************************************************
 *                                                             *
 *   This FILE sets up the IR sensors used in the Micromouse   *
 *                                                             *
 ***************************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "mbed.h"
#include "Infrared.hpp"

// OBJECTS ---------------------------------------------------
DigitalOut ir_led_l(IR_EMIT_L);
DigitalOut ir_led_r(IR_EMIT_R);
DigitalOut ir_led_lf(IR_EMIT_LF);
DigitalOut ir_led_rf(IR_EMIT_RF);

AnalogIn ir_rec_l(IR_REC_L);
AnalogIn ir_rec_r(IR_REC_R);
AnalogIn ir_rec_lf(IR_REC_LF);
AnalogIn ir_rec_rf(IR_REC_RF);

// METHODS ---------------------------------------------------
void initialize_IR() {
  ir_led_l = 0;
  ir_led_r = 0;
  ir_led_lf = 0;
  ir_led_rf = 0;
}

void read_forward(IRData *ird){
  ir_led_l = 1;
  ir_led_r = 1;
  wait_us(LED_ON_US);
  ird->l = ir_rec_l;
  ird->r = ir_rec_r;
  ir_led_l = 0;
  ir_led_r = 0;
}

void read_side(IRData *ird){
  ir_led_lf = 1;
  ir_led_rf = 1;
  wait_us(LED_ON_US);
  ird->lf = ir_rec_lf;
  ird->rf = ir_rec_rf;
  ir_led_lf = 0;
  ir_led_rf = 0;
}

IRData read_all(){
  IRData ird;
  // wait_us(LED_OFF_US);
  read_forward(&ird);
  read_side(&ird);
  // put in a min delay for led cooldown?

  return ird;
}