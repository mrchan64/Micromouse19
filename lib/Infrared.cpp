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
  ir_led_l = 1;
  ir_led_r = 1;
  ir_led_lf = 1;
  ir_led_rf = 1;
}

// void read_forward(IRData *ird){
//   ir_led_l = 0;
//   ir_led_r = 0;
//   wait_us(LED_ON_US);
//   ird->l = ir_rec_l;
//   ird->r = ir_rec_r;
//   ir_led_l = 1;
//   ir_led_r = 1;
// }

// void read_side(IRData *ird){
//   ir_led_lf = 0;
//   ir_led_rf = 0;
//   wait_us(LED_ON_US);
//   ird->lf = ir_rec_lf;
//   ird->rf = ir_rec_rf;
//   ir_led_lf = 1;
//   ir_led_rf = 1;
// }

// IRData read_all(){
//   IRData ird;
//   // wait_us(LED_OFF_US);
//   read_forward(&ird);
//   read_side(&ird);
//   // put in a min delay for led cooldown?

//   return ird;
// }

// ASYNC -----------------------------------------------------

// Timer async_t;
AsyncIRState async_IR_state = IROFF; // 0: off, 1: idle1, 2: reading forward, 3: reading side, 4: idle2
bool dataRefreshed = false;
IRData asyncData;
int cyc_count = 0;

bool cycle_IR_async() {
  cyc_count++;
  switch(async_IR_state){
    case IROFF:
      break;
    case IDLE1:
      ir_led_l = 0;
      ir_led_r = 0;
      // async_t.reset();
      async_IR_state = SCANFORWARD;
      break;
    case SCANFORWARD:
      if(cyc_count == CYCLE_MULTIPLIER){
        asyncData.l = ir_rec_l;
        asyncData.r = ir_rec_r;
        ir_led_l = 1;
        ir_led_r = 1;
        ir_led_lf = 0;
        ir_led_rf = 0;
        async_IR_state = SCANSIDE;
        cyc_count = 0;
      }
      break;
    case SCANSIDE:
      if(cyc_count == CYCLE_MULTIPLIER){
        asyncData.lf = ir_rec_lf;
        asyncData.rf = ir_rec_rf;
        ir_led_lf = 1;
        ir_led_rf = 1;
        async_IR_state = IDLE2;
        dataRefreshed = true;
        cyc_count = 0;
      }
      break;
    case IDLE2:
      if(cyc_count == CYCLE_MULTIPLIER){
        async_IR_state = IDLE1;
        cyc_count = 0;
      }
      break;
  }
  return dataRefreshed;
}

IRData read_IR_async() {
  IRData ird;
  ird.lf = asyncData.lf;
  ird.l = asyncData.l;
  ird.r = asyncData.r;
  ird.rf = asyncData.rf;
  dataRefreshed = false;
  return ird;
}

void off_IR_async() {
  async_IR_state = IROFF;
  ir_led_l = 1;
  ir_led_r = 1;
  ir_led_lf = 1;
  ir_led_rf = 1;
}

void on_IR_async() {
  async_IR_state = IDLE1;
}
