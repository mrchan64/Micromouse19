/******************************************************************
 *                                                                *
 *   This FILE sets up the Debug Methods used in the Micromouse   *
 *                                                                *
 ******************************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "Debug.hpp"

// METHODS ---------------------------------------------------
void print_accel_output(){
  while(true){
    // run_sensor_cycle();
    SensorData sens = get_accel_data();
    pc.printf("A %f %f %f\n", sens.x, sens.y, sens.z);
    wait_ms(20);
  }
}

void test_align_front(){
  IRData dat;
  IRData dat2;
  while(true){
    run_IR_cycle();
    dat = get_current_IR_data();
    dat2 = get_curr_norm_IR_data();

    // aligning to wall
    int stepl = dat2.l>0 ? ceil(dat2.l) : floor(dat2.l);
    int stepr = dat2.r>0 ? ceil(dat2.r) : floor(dat2.r);
    m_left.setStepAboveSpeed(0.0f, -stepl);
    m_right.setStepAboveSpeed(0.0f, -stepr);
  }
}