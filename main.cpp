/***************************************************
 *                                                 *
 *   This FILE runs the main loop for Micromouse   *
 *                                                 *
 ***************************************************/

// AUTHOR: Matthew Chan

#include "mbed.h"
#include "lib/MiscIO.hpp"
#include "lib/Motors.hpp"
#include "lib/Serial.hpp"
// #include "lib/SensorComm.hpp"
#include "lib/Infrared.hpp"
#include "algo/IR_Detect.hpp"
#include "algo/SpeedSetter.hpp"
#include "algo/Calibration.hpp"
#include "algo/WallState.hpp"
#include "algo/MazeNode.hpp"
#include "algo/MazeSolver.hpp"
#include "debug/Debug.hpp"

int pause_ms = 200;

int main() {
  // run_calibration();
  // pre_code();
  // test_align_front();


  set_threshold_manual();
  // print_IR_vals();




  // wait(1);

  /* Prints out values for the calibration*/
  // run_threshold_calibration();
  reset_walls();

  /*SPEEDSETTER*/
  initSpeedSetter();
  resetSpeedSetter();
  setDistanceTicks(10*ONE_CELL_TICKS);
  // setDistanceTicks(2000);
  // setAngleDegrees(UTURN);
  int seq = 0;

  // IR
  on_IR_async();
  Timer t;
  t.start();

  while(true){
    runSpeedSetter();
    run_IR_cycle();
    run_wall_update_cycle();
    float ticks = getCurrDist();
    CellWalls cw = get_curr_walls();
    led.l1 = cw.left;
    led.l3 = cw.front;
    led.l5 = cw.right;
    led.l4 = t.read_ms()%1000 > 500;

    // if(!cw.right && seq==0){
    //   setDistanceTicks(ticks+4900);
    //   seq++;
    // }
    // if(getSpeedCompletion() && seq==1){
    //   resetSpeedSetter();
    //   setAngleDegrees(RIGHT);
    //   seq++;
    // }
    // if(getSpeedCompletion() && seq==2){
    //   resetSpeedSetter();
    //   setDistanceTicks(10*ONE_CELL_TICKS);
    //   seq=0;
    // }
  }






  float lastTime;
  float lastTicks = 0.0f;
  Direction facing = NORTH;
  int mtype = 1;
  // 0 starting forward
  // 1 ex straight
  // 2 nudge forward
  // 3 turn/uturn
  // 4 waiting before nudge
  // 5 waiting after nudge
  // 6 moving before turning
  // 7 moving until IR = -1
  // // on_async_sensor_cycle(true);
  Direction targ = NORTH;

  initialize_maze();
  // while(true){
  //   runSpeedSetter();
  //   run_IR_cycle();
  //   run_wall_update_cycle();
  //   float ticks = getCurrDist();
  //   CellWalls cw = get_curr_walls();
  //   led.l1 = cw.left;
  //   led.l3 = cw.front;
  //   led.l5 = cw.right;
  //   led.l4 = t.read_ms()%1000 > 500;

  //   // if(seq == 0 && !cw.right){
  //   //   // resetSpeedSetter();
  //   //   setDistanceTicks(getCurrDist()+PAST_OPENING_TICKS);
  //   // }

  //   switch(mtype){
  //     case 1:
  //     if(cw.left || cw.right){
  //       setDistanceTicks(ticks);
  //       lastTime = t.read_ms();
  //       mtype = 4;
  //     }
  //     if(cw.front){
  //       visitSquare(facing, cw, false, false);
  //       mtype = 7;
  //     }
  //     if(ticks - lastTicks > ONE_CELL_TICKS){
  //       visitSquare(facing, cw, false, false);
  //     }
  //     break;
  //     case 2:
  //       if(getSpeedCompletion()){
  //         lastTime = t.read_ms();
  //         mtype = 5;
  //         visitSquare(facing, cw, false, false);
  //         // read
  //       }
  //     break;
  //     case 3:
  //       if(getSpeedCompletion()){
  //         resetSpeedSetter();
  //         mtype = 1;
  //         visitSquare(facing, cw, false, false);
  //         setDistanceTicks(100*ONE_CELL_TICKS);
  //       }
  //     break;
  //     case 4:
  //       if(t.read_ms() - lastTime>pause_ms){
  //         mtype = 2;
  //         resetSpeedSetter();
  //         setDistanceTicks(500);
  //       }
  //     break;
  //     case 5:
  //       if(t.read_ms() - lastTime>pause_ms){
  //         mtype = 6;
  //         resetSpeedSetter();
  //         setDistanceTicks(1500);
  //       }
  //     break;
  //     case 6:
  //       if(getSpeedCompletion()){
  //         targ = static_cast<Direction>(getDir());
  //         resetSpeedSetter();
  //         if(targ==facing)setDistanceTicks(10*ONE_CELL_TICKS);
  //         else if((facing+1)%4 == targ)setAngleDegrees(LEFT);
  //         else if((facing+3)%4 == targ)setAngleDegrees(RIGHT);
  //         else setAngleDegrees(UTURN);
  //         facing = targ;
  //         mtype = 3;
  //       }
  //       break;
  //     case 7:
  //       if(get_all_ave().l <-1 || get_all_ave().r<-1){
  //         setDistanceTicks(ticks);
  //         visitSquare(facing, cw, false, false);
  //         mtype = 6;
  //       }  
  //   }

  //   if(getSpeedCompletion()){
  //     if(seq==0){
  //       seq ++;
  //     }
  //     if(seq==1){
  //       seq++;
  //       resetSpeedSetter();
  //       setAngleDegrees(RIGHT);
  //     }
  //   }



  // }
}