/*******************************************************
 *                                                     *
 *   This FILE manages Cell State in the Micromouse    *
 *                                                     *
 *******************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "StateRunner.hpp"

RunnerState currRunState = RUN_IDLE;
bool frontWallNextCell = false;
float lastTicks = 0.0f;
PIDState nextToDo = STRAIGHT;
Direction facing_s = NORTH;
Direction nextDir = NORTH;
bool leftStateChanged = false;
bool rightStateChanged = false;
bool cellConfirmed = false;
bool inNudge = false;
bool justNudged = false;
bool unaligned = false;
bool doingImm = false;
int seq = 0;
CellWalls saved;

// Direction getDir(){

// }

// void setWalls(CellWalls cw){
//   int c = get_c();
//   int r = get_r();
//   if(cw.front){
//     AlgoState::addWall(r, c, facing);
//   }
//   if(cw.left){
//     AlgoState::addWall(r, c, (facing+3)%4);
//   }
//   if(cw.right){
//     AlgoState::addWall(r, c, (facing+1)%4);
//   }

// }

Direction runRight(Direction dir, CellWalls cw){
  if(!cw.right){
    return static_cast<Direction>((dir+3)%4);
  }else{
    return dir;
  }
}

void doConfirmedSeg(float distCM, float currTicks, CellWalls cw){
  // led.l2 = distCM > DIST_CM_WALL + SEACH_SIDE_MARG || (leftStateChanged && rightStateChanged);
  if(distCM > DIST_CM_WALL + SEACH_SIDE_MARG || (leftStateChanged && rightStateChanged)){
    // window for detecting wall change closes.
    if(!cellConfirmed){
      // pass in cw to the maze algorithm
      // nextDir = // get the nextmaze thing
      // visitSquare(facing, cw, false, false);
      // nextDir = static_cast<Direction>(getDir());
      Timer t;
      t.start();
      //nextDir = AlgoState::nextBestMoveToGoal(cw);
      // pc.printf("%f\n\r", t.read_us()/1000000.0f);
      nextDir = runRight(facing_s, cw);
      cellConfirmed = true;

      if(nextDir == (facing_s+1) % 4 || (nextDir+1) % 4 == facing_s){ // 90 deg turn
        if(!doingImm)setDistanceTicks(lastTicks + DIST_CM_TURN);
      }else if(nextDir == (facing_s+2)%4){ // u turn
        // start alignfront
        led.l2 = 1;
        if(cw.front){
          currRunState = RUN_IR_ALIGN_FRONT;
        }else{
          setDistanceTicks(currTicks + DIST_TO_CENTER_FROM_NEW);
        }
      } // otherwise just go
    }
  }
}

void doStraightSC(){
  if(getSpeedCompletion()){ // only triggered when wanting a turn
    resetSpeedSetter();
    if(nextDir == (facing_s+1) % 4){ // turn left
      setAngleDegrees(LEFT);
      currRunState = RUN_LEFT_TURN;
    }else if((nextDir+1)%4 == facing_s){ // turn right
      setAngleDegrees(RIGHT);
      currRunState = RUN_RIGHT_TURN;
    }else{
      setAngleDegrees(UTURN);
      currRunState = RUN_U_TURN;
    }
  }
}

void doEndCellCheck(float currTicks){
  if(currTicks - lastTicks > ONE_CELL_TICKS){
    // explore one forward
    lastTicks = currTicks;
    cellConfirmed = false;
    leftStateChanged = false;
    rightStateChanged = false;
    // setDistanceTicks(currTicks);
  }
}

void doEndTurnSC(CellWalls cw){
  if(getSpeedCompletion()){ // only triggered when wanting a turn
    resetSpeedSetter();
    cellConfirmed = true;
    // pass in cw to the maze algorithm
    // nextDir = //get the next dir
    // nextDir = runRight(facing, cw);
    // if(nextDir == (facing+1) % 4){ // turn left
    //   setAngleDegrees(LEFT);
    //   currRunState = RUN_LEFT_TURN;
    // }else if((nextDir+1)%4 == facing){ // turn right
    //   setAngleDegrees(RIGHT);
    //   currRunState = RUN_RIGHT_TURN;
    // }else if(nextDir == facing){ // straight
      setDistanceTicks(18*ONE_CELL_TICKS);
      // setDistanceTicks(DIST_TURN_CM);
      inNudge = true;
      lastTicks = DIST_TURN_CM;
      if(cw.left && cw.right){
        currRunState = RUN_STRAIGHT_CLOSED;
      }else if(cw.left && !cw.right){
        currRunState = RUN_STRAIGHT_RIGHT_OPEN;
      }else if(!cw.left && cw.right){
        currRunState = RUN_STRAIGHT_LEFT_OPEN;
      }else{
        currRunState = RUN_STRAIGHT_BOTH_OPEN;
      }
      unaligned = true;
      facing_s = nextDir;
    // lastTicks = 
    // }else{
    //   // FIXXXX
    //   setAngleDegrees(UTURN);
    //   currRunState = RUN_U_TURN;
    // }
  }
}

void doIRAlign(float distCM){
  toggleIRRun(distCM < DIST_TO_ALIGN && unaligned);
  toggleFastAlign(distCM < DIST_FAST_ALIGN);
  if(distCM > DIST_TO_ALIGN){
    unaligned = false;
  }
}

void doNudge(float currTicks){
  if(currTicks > NUDGE_DIST && inNudge){
    inNudge = false;
    justNudged = true;
  }
}

void doImmTurn(float currTicks, CellWalls cw){
  if(justNudged){
    justNudged = false;
    // visitSquare(facing, cw, false, false);
    //nextDir = AlgoState::nextBestMoveToGoal(cw);
    nextDir = runRight(facing_s, cw);
    cellConfirmed = true;
    if(nextDir == (facing_s+1) % 4 || (nextDir+1) % 4 == facing_s){ // 90 deg turn
      setDistanceTicks(currTicks + DIST_TURN_IMM);
      doingImm = true;
    }else if(nextDir == (facing_s+2)%4){ // u turn
      // start alignfront
      currRunState = RUN_IR_ALIGN_FRONT;
    } // otherwise just go
  }
}

void runStateRunner(){
  CellWalls cw = get_curr_walls();
  float currTicks = getCurrDist();
  // bool taskFin = getSpeedCompletion();
  float distCM = currTicks - lastTicks;
  // led.l2 = cellConfirmed;
  switch(currRunState){
    case RUN_IDLE:
      currRunState = RUN_STRAIGHT_CLOSED;
      lastTicks = BEG_OFFSET_DIST;
      setDistanceTicks(20*ONE_CELL_TICKS);
    break;

    case RUN_IR_ALIGN_SIDE:
    break;

    case RUN_IR_ALIGN_FRONT:
      if(getSpeedCompletion()){
        currRunState = RUN_U_TURN;
      }
    break;

    case RUN_STRAIGHT_CLOSED: // should complete in here if u turn in middle of hall
      doNudge(currTicks);
      if(inNudge)break;

      doImmTurn(currTicks, cw);

      doStraightSC();

      doConfirmedSeg(distCM, currTicks, cw); // sets the next direction to go

      if(!cw.left){ // left opens guarantees new cell is open left
        lastTicks = currTicks - DIST_CM_WALL;
        leftStateChanged = true;
        cellConfirmed = false;
        currRunState = RUN_STRAIGHT_LEFT_OPEN;
        saved.left = false;
      }
      if(!cw.right){ // right opens guarantees new cell is open right
        lastTicks = currTicks - DIST_CM_WALL;
        rightStateChanged = true;
        cellConfirmed = false;
        currRunState = RUN_STRAIGHT_RIGHT_OPEN;
        saved.right = false;
      }

      doIRAlign(distCM);

      doEndCellCheck(currTicks);

    break;

    case RUN_STRAIGHT_LEFT_OPEN:
      doNudge(currTicks);
      if(inNudge)break;

      doImmTurn(currTicks, cw);

      doStraightSC();

      doConfirmedSeg(distCM, currTicks, cw); // sets the next direction to go

      if(!cw.right){ // right opens guarantees new cell is open
        lastTicks = currTicks - DIST_CM_WALL;
        rightStateChanged = true;
        cellConfirmed = false;
        currRunState = RUN_STRAIGHT_BOTH_OPEN;
        saved.right = false;
      }
      if(cw.left){
        currRunState = RUN_STRAIGHT_CLOSED;
      }

      doIRAlign(distCM);
      doEndCellCheck(currTicks);
    break;

    case RUN_STRAIGHT_RIGHT_OPEN:
      doNudge(currTicks);
      if(inNudge)break;

      doImmTurn(currTicks, cw);

      doStraightSC();

      doConfirmedSeg(distCM, currTicks, cw); // sets the next direction to go

      if(!cw.left){ // right opens guarantees new cell is open
        lastTicks = currTicks - DIST_CM_WALL;
        leftStateChanged = true;
        cellConfirmed = false;
        currRunState = RUN_STRAIGHT_BOTH_OPEN;
        saved.left = false;
      }
      if(cw.right){
        currRunState = RUN_STRAIGHT_CLOSED;
      }

      doIRAlign(distCM);
      doEndCellCheck(currTicks);
    break;

    case RUN_STRAIGHT_BOTH_OPEN:
      doStraightSC();

      doImmTurn(currTicks, cw);

      doConfirmedSeg(distCM, currTicks, cw); // sets the next direction to go
      if(cw.right){
        currRunState = RUN_STRAIGHT_LEFT_OPEN;
      }
      if(cw.left){
        currRunState = RUN_STRAIGHT_RIGHT_OPEN;
      }
      doEndCellCheck(currTicks);
    break;

    case RUN_LEFT_TURN:
      doingImm = false;
      doEndTurnSC(cw);
    break;

    case RUN_RIGHT_TURN:
      doingImm = false;
      doEndTurnSC(cw);
    break;

    case RUN_U_TURN:
    // this just turns the 180
      if(getSpeedCompletion()){
        cellConfirmed = true;
        lastTicks = -5000.0f;
        if(cw.left && cw.right){
          currRunState = RUN_STRAIGHT_CLOSED;
        }else if(cw.left && !cw.right){
          currRunState = RUN_STRAIGHT_RIGHT_OPEN;
        }else if(!cw.left && cw.right){
          currRunState = RUN_STRAIGHT_LEFT_OPEN;
        }else{
          currRunState = RUN_STRAIGHT_BOTH_OPEN;
        }
        unaligned = true;
        setDistanceTicks(18*ONE_CELL_TICKS);
        facing_s = nextDir;
      }
    break;
  }
}