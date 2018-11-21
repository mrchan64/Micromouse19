/************************************************************
 *                                                          *
 *   This FILE manages the Maze Solving of the Micromouse   *
 *                                                          *
 ************************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "MazeSolver.hpp"

// VARIABLES -------------------------------------------------
Location currLoc = {0,0};
bool nextCellKnown = false;
MazePhase currPhase = IDLE;
MazePhase lastStartPhase = SEARCH;

// METHODS ---------------------------------------------------
void initialize_maze(){
  for(int x = 0; x < 16; x++){
    for(int y = 0; y < 16; y++){
      currMaze[x][y] = new Node(x,y);
    }
  }

  // link up down
  for(int x = 0; x < 16; x++){
    for(int y = 0; y < 15; y++){
      currMaze[x][y]->nb[NORTH] = currMaze[x][y+1];
      currMaze[x][y+1]->nb[SOUTH] = currMaze[x][y];
    }
  }

  // link left right
  for(int x = 0; x < 15; x++){
    for(int y = 0; y < 16; y++){
      currMaze[x][y]->nb[EAST] = currMaze[x+1][y];
      currMaze[x+1][y]->nb[WEST] = currMaze[x][y];
    }
  }
}

void populate_wall(Node cell, Direction dir, WallState state){
  if(state == UNSURE)return;
  Node targ_cell = currMaze[cell.loc.x][cell.loc.y];
  targ_cell.resolveWall(dir, (state == WALL));
}

void bfs(Node start, Node end){
  int arrlen = MAZESIZE * MAZESIZE;
  Node queue[arrlen];
  for(int i = 0; i<arrlen; i++){
    queue[i] = NULL;
  }
  for(int x = 0; x < 16; x++){
    for(int y = 0; y < 16; y++){
      currMaze[x][y].bfsexplored = UNEXPLORED;
      currMaze[x][y].bfsScore = 0.0f;
    }
  }
}

void bfs_eval_node(Node currNode){
  
}

void vfield(Node loc){

}

void run_maze_solve_cycle(){
  run_IR_cycle();
  SensorData g_dat = read_gyro();
  if(check_pause_state()) currPhase = ERROR;
  
  /* STATE MACHINE */
  switch(currPhase){
    case IDLE:
    break;
    case ERROR:
    runError();
    break;
    case SEARCH:
    runSearch();
  }
}

void runIdle(){
  // do nothing
  return;
}

/* THIS IS THE CODE RUN IN THE ERROR STATE
 *
 * 1. Whenever error is detected, restarts the cycle
 * 2. Waits for no error (put down)
 * 3. Waits 1 second
 * 4. Waits for hand in front
 * 5. Waits for hand to leave
 * 6. Waits 1 second
 * 7. Begins last start phase
 */
uint8_t ecounter = 0;
Timer int_1;
void runError(){
  if(check_pause_state()){
    ecounter = 0;
    led.on(0,0,0,0,0,1);
    m_left.setSpeed(0.0f); m_right.setSpeed(0.0f);
  }else{
    switch(ecounter){
      case 0:
      ecounter = 1;
      led.off();
      int_1.reset();
      int_1.start();
      break;
      case 1:
      if(int_1.read_ms() >= 1000)ecounter = 2;
      break;
      case 2:
      led.on(0,0,0,0,1,0);
      IRData ird = read_IR_async();
      if(ird.l > START_IR_DIST && ird.r > START_IR_DIST)ecounter = 3;
      break;
      case 3:
      led.on(0,0,0,1,0,0);
      IRData ird = read_IR_async();
      if(ird.l < f_thresh.l && ird.r > f_thresh.r){
        ecounter = 4;
        int_1.reset();
        int_1.start();
      }
      break;
      case 4:
      led.off();
      if(int_1.read_ms()>1000){
        currPhase = lastStartPhase;
        reset_walls();
        reset_nav();
      }
      break;
    }
  }
}