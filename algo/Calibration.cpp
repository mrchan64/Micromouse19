/********************************************************************
 *                                                                  *
 *   This FILE manages the starting calibration of the Micromouse   *
 *                                                                  *
 ********************************************************************/

// AUTHOR: Matthew Chan

#include "Calibration.hpp"
#include "../lib/Serial.hpp"
#include "IR_Detect.hpp"

// METHODS ---------------------------------------------------

void pre_code(){
  pc.printf("\n");
  wait(REST_TIME_US);
  cycle_leds(.25f);
  initialize_IR();
  on_IR_async();
  normalize_IR_sensors();
  uint8_t s_e_s = read_sensor_error_state();
  if (s_e_s) pc.printf("Sensor error: %d\n", s_e_s);
  else normalize_sensors();
  wait(REST_TIME_US);
  float voltage = get_voltage();
  pc.printf("Voltage: %f\n", voltage);
  bool low_v = voltage < V_MIN;
  if (s_e_s || low_v){ // stop all operation
    while(true){
      led.on(1,1,1,1,1);
      wait_ms(300);
      led.off();
      wait_ms(300);
    }
  }
  cycle_leds(.25f);
}

/** 0. Start pointing backward in cell
  * 1. Move forward (4.5cm = .25 cell)
  * 2. Do scaling
  * 3. Move backward ('')
  * 4. Rotate 90 (pointing right)
  * 5. Thresholding for left
  * 6. Rotate 90 (pointing front)
  * 7. Thresholding for front
  * 8. Rotate 90 (pointing left)
  * 9. Thresholding for right
  */
void mid_code(){
  move_dist(2.0f);
  led.on(0,1,0,0,0);
  wait(REST_TIME_US);
  scale_IR_sensors();
  wait(REST_TIME_US);
  move_dist(-2.0f);
  wait(REST_TIME_US);
  // turn_90();
  led.on(0,0,1,0,0);
  wait(REST_TIME_US);
  set_IR_threshold_left();
  wait(REST_TIME_US);
  // turn_90();
  led.on(0,0,0,1,0);
  wait(REST_TIME_US);
  set_IR_threshold_forward();
  wait(REST_TIME_US);
  // turn_90();
  led.on(0,0,0,1,0);
  wait(REST_TIME_US);
  set_IR_threshold_right();
  wait(REST_TIME_US);
  led.on(0,0,0,0,1);
}

void post_code(){
  while(!check_pause_state()) {wait_us(1);}
  led.on(0,1,0,0,1);
  while(check_pause_state()) {wait_us(1);}
  led.off();
  wait_us(REST_TIME_US);
  cycle_leds(.25f);
}

// Turns 90 degrees cclockwise (left)
// void turn_90(){
//   Timer t;
//   int time;
//   int lasttime;
//   float timed;
//   float totalx = 0.0f;
//   float totaly = 0.0f;
//   float totalz = 0.0f;
//   SensorData gdat;
//   int c2 = 0;
//   t.start();
//   lasttime = t.read_us();
//   while(totalz < 90){
//     m_right.setSpeed(.08);
//     m_left.setSpeed(-.08);
//     c2++;
//     gdat = read_gyro();
//     time = t.read_us();
//     timed = (float) (time-lasttime)/1000000.0f;
//     totalx += timed * gdat.x;
//     totaly += timed * gdat.y;
//     totalz += timed * gdat.z;
//     lasttime = time;

//   }
//   t.stop();

//   m_left.setSpeed(0);
//   m_right.setSpeed(0);
// }

void move_dist(float dist_cm){
  float dist_ticks = dist_cm / (WHEEL_DIAM * PI) * ONE_REV;
  move_ticks(dist_ticks);
}

void move_ticks(float dist_ticks){
  Timer t; t.reset(); t.start();
  m_left.resetEncoder(); m_right.resetEncoder();
  if(dist_ticks > 0){
    while(m_left.getTicks() < dist_ticks){
      int time = t.read_us();
      float speed1 = time * RAMP_SPEED + MIN_SPEED;
      float speed2 = MAX_SPEED;
      float speed3 = (dist_ticks - m_left.getTicks()) / ONE_REV * 1.3f + MIN_SPEED;
      float speedf = (speed1 < speed2) ? ((speed1 < speed3) ? speed1 : speed3) : speed2;
      m_left.setSpeed(speedf/2);
      m_right.setSpeed(speedf/2);
    }
  }else{
    while(m_left.getTicks() > dist_ticks){
      int time = t.read_us();
      float speed1 = time * RAMP_SPEED + MIN_SPEED;
      float speed2 = MAX_SPEED;
      float speed3 = (-dist_ticks + m_left.getTicks()) / ONE_REV * 1.3f + MIN_SPEED;
      float speedf = (speed1 < speed2) ? ((speed1 < speed3) ? speed1 : speed3) : speed2;
      m_left.setSpeed(-speedf/2);
      m_right.setSpeed(-speedf/2);
    }
  }
  m_left.setSpeed(0.0f);
  m_right.setSpeed(0.0f);
}

void run_calibration(){
  // start facing wall
  pre_code();
  mid_code();
  post_code();
}

void align_front(float dist){
  // remember that ir level is inversely proportional to distance to wall
  IRData dat2 = get_curr_norm_IR_data();

  dat2.l -= dist;
  dat2.r -= dist;

  // aligning to wall
  int stepl = dat2.l>0 ? ceil(dat2.l/.005) : floor(dat2.l/.005);
  int stepr = dat2.r>0 ? ceil(dat2.r/.005) : floor(dat2.r/.005);
  m_left.setStepAboveSpeed(0.0f, -stepl);
  m_right.setStepAboveSpeed(0.0f, -stepr);
}


// CODE FOR PRESETTING FTHRESH

void run_threshold_calibration(){
  /*
  1. Cycles leds
  2. initializes (pointing toward wall);
  3. led2 turns on, move mouse toward wall (sets forward scaling)
  4. led3 turns on, point left of mouse towards open
  5. led4 turns on, point front of mouse towards open
  6. led5 turns on, point right of mouse towards open
  7. finished when leds turns off
  */
  // Timer cal;
  // cal.start();
  cycle_leds(.25f);
  initialize_IR();
  on_IR_async();
  normalize_IR_sensors();

  led.on(0,1,0,0,0);
  wait(REST_TIME_US);
  scale_IR_sensors();
  led.on(0,0,1,0,0);
  wait(REST_TIME_US);
  set_IR_threshold_left();
  led.on(0,0,0,1,0);
  wait(REST_TIME_US);
  set_IR_threshold_forward();
  led.on(0,0,0,0,1);
  wait(REST_TIME_US);
  set_IR_threshold_right();
  led.off();

  print_threshold_vals();
}

void set_threshold_manual(){
  initialize_IR();
  setIRConstantsManual(PREV_S, PREV_F, PREV_B);
  on_IR_async();
  pc.printf("V: %f Volts\n\r", get_voltage());
  cycle_leds(.25);
  cycle_leds(.25);
  uint8_t s_e_s = read_sensor_error_state();
  if (s_e_s) pc.printf("Sensor error: %d\n", s_e_s);
  else normalize_sensors();
}

void print_IR_vals(){
  cycle_leds(.25f);
  initialize_IR();
  on_IR_async();
  Timer t;
  t.start();
  while(true){
    run_IR_cycle();
    if(t.read_ms() > 3000){
      t.reset();
      pc.printf("%f %f %f %f\n\r", norm_total.lf / REVIEW_WINDOW, norm_total.l / REVIEW_WINDOW, norm_total.r / REVIEW_WINDOW, norm_total.rf / REVIEW_WINDOW);
    }
    wait_us(6);
  }
}