/***********************************************************
 *                                                         *
 *   This FILE sets up the motors used in the Micromouse   *
 *                                                         *
 ***********************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "Motors.hpp"

// MOTOR CLASS -----------------------------------------------
Motor::Motor(char side): 
  channel_a( (side=='l') ? MOTOR_LEFT_A : MOTOR_RIGHT_A), 
  channel_b( (side=='l') ? MOTOR_LEFT_B: MOTOR_RIGHT_B),
  enc( (side=='l') ? ENCODER_LEFT_A:ENCODER_RIGHT_A, 
    (side=='l') ? ENCODER_LEFT_B: ENCODER_RIGHT_B)
{
  // if(side == 'l'){
  //   channel_a = new PwmOut(MOTOR_LEFT_A);
  //   channel_b = new PwmOut(MOTOR_LEFT_B);
  //   enc = new Encoder(ENCODER_LEFT_A, ENCODER_LEFT_B);
  // }else if(side == 'r'){
  //   channel_a = new PwmOut(MOTOR_RIGHT_A);
  //   channel_b = new PwmOut(MOTOR_RIGHT_B);
  //   enc = new Encoder(ENCODER_RIGHT_A, ENCODER_RIGHT_B);
  // }else{
  //   return;
  // }

  curr_speed = 0.0f;
  enc.reset();
  channel_a.write(0);
  channel_b.write(0);
  channel_a.period_us(PERIOD_US);
  channel_b.period_us(PERIOD_US);
}

void Motor::setSpeed(float speed){
  if(speed==0.0f){
    channel_a.write(0);
    channel_b.write(0);
    curr_speed = 0.0f;
    return;
  }
  int mult = speed > 0 ? 1 : -1;
  bool pos = speed > 0;
  speed = abs(speed);
  speed = speed - fmod(speed, SPEED_STEP) + SPEED_STEP/2;
  speed = speed > MAX_SPEED ? MAX_SPEED : speed;
  speed = speed < MIN_SPEED ? MIN_SPEED : speed;
  curr_speed = speed * mult;
  if(pos){
    channel_a.write(speed);
    channel_b.write(0);
  }else{
    channel_a.write(0);
    channel_b.write(speed);
  }
}

float Motor::getSpeed(){
  return curr_speed;
}

float Motor::stepUp(){
  float speed = curr_speed == 0.0f ? MIN_SPEED + .001 : curr_speed + SPEED_STEP;
  int mult = speed > 0 ? 1 : -1;
  bool pos = speed > 0;
  speed = abs(speed);
  speed = speed - fmod(speed, SPEED_STEP) + SPEED_STEP/2;
  speed = speed > MAX_SPEED ? MAX_SPEED : speed;
  speed = speed < MIN_SPEED ? 0.0f : speed;
  curr_speed = speed * mult;
  if(pos){
    channel_a.write(speed);
    channel_b.write(0);
  }else{
    channel_a.write(0);
    channel_b.write(speed);
  }
  return curr_speed;
}

float Motor::stepDown(){
  float speed = curr_speed == 0.0f ? -1 * MIN_SPEED - .001 : curr_speed - SPEED_STEP;
  int mult = speed > 0 ? 1 : -1;
  bool pos = speed > 0;
  speed = abs(speed);
  speed = speed - fmod(speed, SPEED_STEP) + SPEED_STEP/2;
  speed = speed > MAX_SPEED ? MAX_SPEED : speed;
  speed = speed < MIN_SPEED ? 0.0f : speed;
  curr_speed = speed * mult;
  if(pos){
    channel_a.write(speed);
    channel_b.write(0);
  }else{
    channel_a.write(0);
    channel_b.write(speed);
  }
  return curr_speed;
}

void Motor::setStepAboveSpeed(float speed, int step){
  int mult = speed > 0 ? 1 : -1;
  speed = abs(speed);
  speed = speed - fmod(speed, SPEED_STEP) + SPEED_STEP/2;
  speed = speed > MAX_SPEED ? MAX_SPEED : speed;
  speed = speed < MIN_SPEED ? MIN_SPEED : speed;
  curr_speed = speed * mult;
  if(step > 0){
    for(int i = 0; i<step; i++){
      speed = curr_speed == 0.0f ? MIN_SPEED + .001 : curr_speed + SPEED_STEP;
      mult = speed > 0 ? 1 : -1;
      speed = abs(speed);
      speed = speed - fmod(speed, SPEED_STEP) + SPEED_STEP/2;
      speed = speed > MAX_SPEED ? MAX_SPEED : speed;
      speed = speed < MIN_SPEED ? 0.0f : speed;
      curr_speed = speed * mult;
    }
  }else{
    for(int i = 0; i<step; i++){
      speed = curr_speed == 0.0f ? -1 * MIN_SPEED - .001 : curr_speed - SPEED_STEP;
      mult = speed > 0 ? 1 : -1;
      speed = abs(speed);
      speed = speed - fmod(speed, SPEED_STEP) + SPEED_STEP/2;
      speed = speed > MAX_SPEED ? MAX_SPEED : speed;
      speed = speed < MIN_SPEED ? 0.0f : speed;
      curr_speed = speed * mult;
    }
  }
  bool pos = curr_speed > 0;
  if(pos){
    channel_a.write(speed);
    channel_b.write(0);
  }else{
    channel_a.write(0);
    channel_b.write(speed);
  }
}

void Motor::moveDistance(float dist_cm){
  // fill this out, figure out the ramp speed stuff
}

void Motor::moveTicks(float dist_ticks){
  // same as ^^^
}

void Motor::resetEncoder(){
  enc.reset();
}

int Motor::getTicks(){
  return enc.read();
}

int Motor::getDist(){
  return enc.read();
}