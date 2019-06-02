/*******************************************************
 *                                                     *
 *   This FILE manages Motor Speed in the Micromouse   *
 *                                                     *
 *******************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "mbed.h"
#include "SpeedSetter.hpp"
#include "../lib/Serial.hpp"
#include "../lib/MiscIO.hpp"

// MOVING STUFF
PIDState currMovementState = STRAIGHT;
float targDist = 0.0f;
float currDist = 0.0f;
float lastDist = 0.0f;
float wheelDiff = 0.0f;
float currSpeed = 0.0f;
float lastTime = 0.0f;
float totalError = 0.0f;
float timeDiff = 0.0f;
float derivError = 0.0f;
Timer t4;
float error_times[NUM_DERIV_TIMES];
int error_counter = 0;

float wall_diffs[NUM_DERIV_TIMES];
float wallDiff = 0.0f;
bool lastWallDiffAvail = false;
float wheelOffset = 0.0f;

// CONSTANT SPEED
float tps_times[NUM_DERIV_TIMES];
float tps_integ = 0.0f;

float error_tps_times[NUM_DERIV_TIMES];
float targtps = 0.0f;
float currtps = 0.0f;
float totaltpsError = 0.0f;
float derivtpsError = 0.0f;

// TURNING
// float targDegrees = 0.0f;
// float totalDegrees = 0.0f;
// float error_degrees[NUM_DERIV_TIMES];
float error_sub[NUM_DERIV_TIMES];
float subWheelDist = 0.0f;
float subTotalError = 0.0f;
float subDerivError = 0.0f;

// SPEED GATING
bool gated = true;
bool useIR = true;
bool fastAlign = true;


// CellWalls, 

// HELPER METHODS --------------------------------------------
void updateVals(){
  float currTime = t4.read();

  timeDiff = currTime - lastTime;

  if(currMovementState == STRAIGHT){\
    float lt = (float)m_left.getTicks();
    float rt = (float)m_right.getTicks();
    currDist = (lt + rt)/2.0f;
    wheelDiff = lt - rt;
    float currError = targDist - currDist;

    if(useIR && currError > CONST_DIST){
      // if use ir set wheeldiff to 0
      CellWalls cw = get_curr_walls();
      wallDiff = 0.0f;
      if(cw.left) wallDiff += currWallVals.left;
      if(cw.right) wallDiff -= currWallVals.right;
      // if(cw.left && cw.right) wallDiff *= .5f;
      // wallDiff *= IR_STRAIGHT_MULT;
      wheelOffset = wheelDiff;
    }

    wheelDiff -= wheelOffset;
  }else if(currMovementState == UTURN){
    float lt = (float)m_left.getTicks();
    float rt = -(float)m_right.getTicks();
    currDist = (lt + rt)/2.0f;
    wheelDiff = lt - rt;
  }else{
    if(currMovementState == LEFT){
      currDist = m_right.getTicks();
      subWheelDist = m_left.getTicks();
    }else{
      currDist = m_left.getTicks();
      subWheelDist = m_right.getTicks();
    }
    // SensorData g_dat = get_gyro_data();
    // totalDegrees += g_dat.z / timeDiff;
  }

  currtps = (currDist - lastDist) / timeDiff;
  lastDist = currDist;

  tps_integ += currtps - tps_times[error_counter];
  tps_times[error_counter] = currtps;

  lastTime = currTime;
}

void updateSpeeds(){ // use after updatevals
  if(currMovementState == STRAIGHT){

    // PID
    // float currTime = t.read();
    float currError = targDist - currDist;
    derivError = (currError - error_times[error_counter]);
    totalError += derivError;
    error_times[error_counter] = currError;

    float targspeed = PROP_K * currError + INTEG_K * totalError + DERIV_K * derivError;
    targspeed *= SPEED_MULT;

    // extra correction boost when in close range
    if(abs(currError) < BOOST_RANGE){
      if(targspeed < 0) targspeed -= CORRECTION_BOOST;
      else if(targspeed > 0) targspeed += CORRECTION_BOOST;
    }



    // set differential step

    int step;

    if(useIR && currError > CONST_DIST){ // automatically switches to wheel diff mode within const dist
      // step = ceil(wallDiff*wallDiff);


      float wallDiffDeriv = (wallDiff - wall_diffs[error_counter])*WALL_DIFF_DERIV_MULT;
      wall_diffs[error_counter] = wallDiff;
      if(!lastWallDiffAvail)wallDiffDeriv = 0.0f;


      step = ceil(wallDiffDeriv*wallDiffDeriv);
      if(wallDiffDeriv==0){
        m_left.setSpeed(targspeed);
        m_right.setSpeed(targspeed);
      }else if(wallDiffDeriv > 0){ // left is ahead
        m_left.setStepAboveSpeed(targspeed, -step);
        m_right.setStepAboveSpeed(targspeed, step);
      }else { // right is ahead
        m_left.setStepAboveSpeed(targspeed, step);
        m_right.setStepAboveSpeed(targspeed, -step);
      }
    }else{
      float unit = (wheelDiff / UNIT_STEP_RANGE);
      step = ceil(unit*unit);
      if(wheelDiff==0){
        m_left.setSpeed(targspeed);
        m_right.setSpeed(targspeed);
      }else if(wheelDiff > 0){ // left is ahead
        m_left.setStepAboveSpeed(targspeed, -step);
        m_right.setStepAboveSpeed(targspeed, step);
      }else { // right is ahead
        m_left.setStepAboveSpeed(targspeed, step);
        m_right.setStepAboveSpeed(targspeed, -step);
      }
    }

    if(!lastWallDiffAvail && error_counter + 1 == NUM_DERIV_TIMES) lastWallDiffAvail = true;
    error_counter = (error_counter + 1)%NUM_DERIV_TIMES;


  }else if(currMovementState == UTURN){
    // PID
    // float currTime = t.read();
    float currError = RIGHT_ANG_TICKS - currDist;
    derivError = (currError - error_times[error_counter]);
    totalError += derivError;
    error_times[error_counter] = currError;
    error_counter = (error_counter + 1)%NUM_DERIV_TIMES;

    float targspeed = PROP_K * currError + INTEG_K * totalError + DERIV_K * derivError;
    targspeed *= SPEED_MULT;

    // extra correction boost when in close range
    if(abs(currError) < BOOST_RANGE){
      if(targspeed < 0) targspeed -= CORRECTION_BOOST;
      else if(targspeed > 0) targspeed += CORRECTION_BOOST;
    }



    // set differential step
    float unit = (wheelDiff / UNIT_STEP_RANGE);
    int step = ceil(unit*unit);

    if(wheelDiff==0){
      m_left.setSpeed(targspeed);
      m_right.setSpeed(-targspeed);
    }else if(wheelDiff > 0){ // left is ahead
      m_left.setStepAboveSpeed(targspeed, -step);
      m_right.setStepAboveSpeed(-targspeed,-step);
    }else { // right is ahead
      m_left.setStepAboveSpeed(targspeed, step);
      m_right.setStepAboveSpeed(-targspeed, step);
    }
  }else{
    // int multiplier = -1;
    // if(currMovementState == LEFT) multiplier = 1;

    float currError = (float) /*multiplier **/ (RIGHT_ANG_TICKS - currDist);
    derivError = (currError - error_times[error_counter]);
    totalError += derivError;
    error_times[error_counter] = currError;

    float targspeed = PROP_K * currError + INTEG_K * totalError + DERIV_K * derivError;
    targspeed *= SPEED_MULT;

    if(abs(currError) < BOOST_RANGE){
      if(targspeed < 0) targspeed -= CORRECTION_BOOST;
      else if(targspeed > 0) targspeed += CORRECTION_BOOST;
    }

    if(currMovementState == LEFT) m_right.setSpeed(targspeed);
    else m_left.setSpeed(targspeed);

    // nondominant wheel
    currError = 0 - subWheelDist;
    subDerivError = (currError - error_sub[error_counter]);
    subTotalError += subDerivError;
    error_sub[error_counter] = currError;

    targspeed = PROP_K * currError + INTEG_K * subTotalError + DERIV_K * subDerivError;
    targspeed *= SPEED_MULT;
    // pc.printf("%.3f\n\r", targspeed);

    if(abs(currError) < BOOST_RANGE){
      if(targspeed < 0) targspeed -= CORRECTION_BOOST;
      else if(targspeed > 0) targspeed += CORRECTION_BOOST;
    }

    if(currMovementState == LEFT) m_left.setSpeed(targspeed);
    else m_right.setSpeed(targspeed);


    error_counter = (error_counter + 1)%NUM_DERIV_TIMES;


  }



}

void updateSpeedPID () {
  float currError = targtps - currtps;
  derivtpsError = currError - error_tps_times[error_counter];
  totaltpsError += derivtpsError;
  // pc.printf("%.2f\n\r", totaltpsError);
  error_tps_times[error_counter] = currError;

  float targspeed = SPID_PROP_K * currError + SPID_INTEG_G * totaltpsError + SPID_DERIV_K * derivtpsError;
  targspeed *= SPID_MULT;

  // differential step
  int step;

  if(useIR && currError > CONST_DIST){ // automatically switches to wheel diff mode within const dist
    // step = ceil(wallDiff*wallDiff);

    float wallDiffDeriv = wallDiff - wall_diffs[error_counter];
    wall_diffs[error_counter] = wallDiff;
    if(!lastWallDiffAvail)wallDiffDeriv = 0.0f;
    // pc.printf("%f\n\r", wallDiffDeriv);

    // step = ceil(wallDiffDeriv*wallDiffDeriv);
    step = IR_CORR_STEP;
    if(fastAlign) step *= FAST_CORR_MULT;
    if(wallDiffDeriv==0){
      m_left.setSpeed(targspeed);
      m_right.setSpeed(targspeed);
    }else if(wallDiffDeriv > 0){ // left is ahead
      m_left.setStepAboveSpeed(targspeed, -step);
      m_right.setStepAboveSpeed(targspeed, step);
    }else { // right is ahead
      m_left.setStepAboveSpeed(targspeed, step);
      m_right.setStepAboveSpeed(targspeed, -step);
    }
  }else if(currMovementState == FALIGN){
    IRData ird = get_all_ave();
    float currError = targtps - currtps;
    derivtpsError = currError - error_tps_times[error_counter];
    totaltpsError += derivtpsError;
    // pc.printf("%.2f\n\r", totaltpsError);
    error_tps_times[error_counter] = currError;

    float targspeed = SPID_PROP_K * currError + SPID_INTEG_G * totaltpsError + SPID_DERIV_K * derivtpsError;
    targspeed *= SPID_MULT;

    int step = FALIGN_STEP;
    if(ird.l - ird.r > 0) step=-step;
    // if(currError > 0)step = -step;
    m_left.setStepAboveSpeed(targspeed, step);
    m_right.setStepAboveSpeed(targspeed, -step);
  }else{
    float unit = (wheelDiff / UNIT_STEP_RANGE);
    step = ceil(unit*unit);
    if(wheelDiff==0){
      m_left.setSpeed(targspeed);
      m_right.setSpeed(targspeed);
    }else if(wheelDiff > 0){ // left is ahead
      m_left.setStepAboveSpeed(targspeed, -step);
      m_right.setStepAboveSpeed(targspeed, step);
    }else { // right is ahead
      m_left.setStepAboveSpeed(targspeed, step);
      m_right.setStepAboveSpeed(targspeed, -step);
    }
  }

  if(!lastWallDiffAvail && error_counter + 1 == NUM_DERIV_TIMES) lastWallDiffAvail = true;
  error_counter = (error_counter + 1)%NUM_DERIV_TIMES;
}

// METHODS ---------------------------------------------------
void initSpeedSetter() {
  t4.start();
}

void setDistanceTicks(float ticks) {
  currMovementState = STRAIGHT;
  targDist = ticks;
}

void setAngleDegrees(PIDState dir) { // left = true, right = false;
  if(dir != STRAIGHT)currMovementState = dir;
  // targDegrees = angle;
}

void setFrontAlign(){
  currMovementState = FALIGN;
  t4.reset();
  t4.start();

}

// void setSpeedTPS(float tps) {
//   targtps = tps;
// }

void resetSpeedSetter(){
  targDist = 0.0f;
  currDist = 0.0f;
  lastDist = 0.0f;
  wheelDiff = 0.0f;
  currSpeed = 0.0f;
  // lastTime = 0.0f;
  totalError = 0.0f;
  tps_integ = 0.0f;

  targtps = 0.0f;
  currtps = 0.0f;
  totaltpsError = 0.0f;
  derivtpsError = 0.0f;

  // targDegrees = 0.0f;
  // totalDegrees = 0.0f;
  subWheelDist = 0.0f;
  subTotalError = 0.0f;
  subDerivError = 0.0f;
  wheelOffset = 0.0f;
  m_left.resetEncoder();
  m_right.resetEncoder();
  // t4.reset();

  wallDiff = 0.0f;
  lastWallDiffAvail = false;

  // reset derivative window
  for(int i = 0; i<NUM_DERIV_TIMES; i++){
    tps_times[i] = 0;
    error_times[i] = 0;
    error_sub[i] = 0;
    error_tps_times[i] = 0;
    wall_diffs[i] = 0;
    // error_degrees[i] = 0;
  }
  error_counter = 0;
}

void runSpeedSetter() {
  updateVals();
  if(currMovementState == STRAIGHT){
    if(targDist - currDist > CONST_DIST){
      targtps = CONST_SPEED;
      updateSpeedPID();
      // led.on(1,0,1,0,1);
    }else if(currDist - targDist > CONST_DIST){
      targtps = -CONST_SPEED;
      updateSpeedPID();
      // led.on(0,1,0,1,0);
    }else{
      updateSpeeds();
      // led.on(0,0,1,0,0);
    }
  }else if(currMovementState == FALIGN){
    updateSpeedPID();
  }else{
    IRData ird = get_all_ave();
    float ave = (ird.l + ird.r)/2;
    if(ave<0)targtps = CONST_SPEED;
    else targtps = -CONST_SPEED;
    updateSpeeds();
  }
}

bool getSpeedCompletion() {

  // check for completion
  if(currMovementState == STRAIGHT)return abs(currDist - targDist) < TICK_COMPLETION_MARGIN;
  else if(currMovementState == FALIGN) return t4.read_ms() > FALIGN_TIME_MS;
  else return abs(currDist - RIGHT_ANG_TICKS) < TICK_COMPLETION_MARGIN;
  // bool conda = derivError == 0;
  // bool condb = currtps == 0;
  // return conda && condb;
}

float getCurrDist(){
  return currDist;
}

void toggleIRRun(bool on){
  useIR = on;
}

void toggleFastAlign(bool on){
  fastAlign = on;
}