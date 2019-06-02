/*******************************************************
 *                                                     *
 *   This FILE manages Cell State in the Micromouse    *
 *                                                     *
 *******************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "StateRunner.hpp"

RunnerState currRunState = IDLE;
bool frontWallNextCell = false;
bool currCellConfirmed = false;
float lastTicks = 0.0f;
PIDState nextToDo = STRAIGHT;
Direction facing = NORTH;

void runStateRunner(){
  CellWalls cw = get_curr_walls();
  float currTicks = getCurrDist();
  bool taskFin = getSpeedCompletion();
  switch(currRunState){
    case IDLE:
    currRunState = RUN_STRAIGHT_CLOSED;
    setDistanceTicks(20*ONE_CELL_TICKS);
    break;
    case RUN_IR_ALIGN_SIDE:
    break;
    case RUN_IR_ALIGN_FRONT:
    break;
    case RUN_STRAIGHT_CLOSED:
    if(!cw.left){
      currRunState = RUN_STRAIGHT_LEFT_OPEN;
    }
    if(!cw.right){
      currRunState = RUN_STRAIGHT_RIGHT_OPEN;
    }
    if(currTicks - lastTicks > ONE_CELL_TICKS){
      // explore one forward
      lastTicks = currTicks;
    }
    break;
    case RUN_STRAIGHT_LEFT_OPEN:
    if(!cw.right){
      currRunState = RUN_STRAIGHT_BOTH_OPEN;
    }
    if(cw.left){
      currRunState = RUN_STRAIGHT_CLOSED;
    }
    break;
    case RUN_STRAIGHT_RIGHT_OPEN:
    if(!cw.left){
      currRunState = RUN_STRAIGHT_BOTH_OPEN;
    }
    if(cw.right){
      currRunState = RUN_STRAIGHT_CLOSED;
    }
    break;
    case RUN_STRAIGHT_BOTH_OPEN:
    if(cw.right){
      currRunState = RUN_STRAIGHT_LEFT_OPEN;
    }
    if(cw.left){
      currRunState = RUN_STRAIGHT_RIGHT_OPEN;
    }
    break;
    case RUN_LEFT_TURN:
    break;
    case RUN_RIGHT_TURN:
    break;
  }
}