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
Node* currNode;
bool nextCellKnown = false;
MazePhase currPhase = IDLE;
MazePhase lastStartPhase = SEARCH;
Node currMaze[MAZESIZE][MAZESIZE];
Node* targets[4];
int dftarg[MAZESIZE][MAZESIZE];
int dfme[MAZESIZE][MAZESIZE];
Node nullNode;
Node* toVisit[ARRLEN];
int toVisitStart = 0;
int toVisitEnd = 0;

// METHODS ---------------------------------------------------
void initialize_maze(){
  // create the nullNode;
  nullNode.setXY(-1,-1);
  int counter = 0;

  for(int x = 0; x < 16; x++){
    for(int y = 0; y < 16; y++){
      currMaze[x][y].setXY(x, y);
      for(int i =0 ;i<4; i++){
        currMaze[x][y].nb[i] = &nullNode;
        currMaze[x][y].previous = &nullNode;
      }
      toVisit[counter++] = &currMaze[x][y];
    }
  }

  // link up down
  for(int x = 0; x < 16; x++){
    for(int y = 0; y < 15; y++){
      currMaze[x][y].nb[NORTH] = &(currMaze[x][y+1]);
      currMaze[x][y+1].nb[SOUTH] = &(currMaze[x][y]);
    }
  }

  // link left right
  for(int x = 0; x < 15; x++){
    for(int y = 0; y < 16; y++){
      currMaze[x][y].nb[EAST] = &(currMaze[x+1][y]);
      currMaze[x+1][y].nb[WEST] = &(currMaze[x][y]);
    }
  }

  targets[0] = &currMaze[7][7];
  targets[1] = &currMaze[7][8];
  targets[2] = &currMaze[8][7];
  targets[3] = &currMaze[8][8];
  currNode = &currMaze[0][0];
}

void populate_wall(Node cell, Direction dir, WallState state){
  if(state == UNSURE)return;
  Node targ_cell = currMaze[cell.loc.x][cell.loc.y];
  targ_cell.resolveWall(dir, (state == WALL));
}

void dfieldtarg(){
  for(int i = 0; i<MAZESIZE; i++){
    for(int j = 0; j<MAZESIZE; j++){
      dftarg[i][j] = 1000;
    }
  }
  Node* heap[ARRLEN];
  int heapStart = 0;
  int heapEnd = 0;
  for(int j = 0; j< 4; j++){
    targets[j]->bfsexplored = EXPLORED;
    dftarg[targets[j]->loc.x][targets[j]->loc.y]= 0;
  }
  for(int j = 0; j< 4; j++){
    Node* targ = targets[j];
    for(int i = 0; i<4; i++){
      if(targ->nb[i]!=&nullNode && (targ->nb[i]->bfsexplored)==UNEXPLORED) heap[heapEnd++] = (targ->nb[i]);
    }
  }
  while(heapEnd - heapStart>0){
    Node* currn = heap[heapStart++];
    currn->bfsexplored = EXPLORED;
    int valueLowest = 1000;
    for(int i = 0; i<4; i++){
      if(currn->nb[i]!=&nullNode && currn->nb[i]->bfsexplored==UNEXPLORED && currn->nb[i]->visited==UNVISITED){
        heap[heapEnd++] = currn->nb[i];
        currn->nb[i]->bfsexplored = EXPLORED;
      }
      if(currn->nb[i]!=&nullNode){
        if(dftarg[currn->nb[i]->loc.x][currn->nb[i]->loc.y]<valueLowest){
          valueLowest = dftarg[currn->nb[i]->loc.x][currn->nb[i]->loc.y];
        }
      }
    }
    dftarg[currn->loc.x][currn->loc.y]=valueLowest==1000 ? valueLowest : valueLowest+1;
  }
  for(int i = 0; i<MAZESIZE; i++){
    for(int j = 0; j<MAZESIZE; j++){
      currMaze[i][j].bfsexplored = UNEXPLORED;
    }
  }
}

void dfieldme(){
  for(int i = 0; i<MAZESIZE; i++){
    for(int j = 0; j<MAZESIZE; j++){
      dfme[i][j] = 1000;
    }
  }
  Node* heap[ARRLEN];
  int heapStart = 0;
  int heapEnd = 0;
  for(int j = 0; j< 4; j++){
    targets[j]->bfsexplored = EXPLORED;
    dftarg[targets[j]->loc.x][targets[j]->loc.y]= 0;
  }
  for(int j = 0; j< 4; j++){
    Node* targ = targets[j];
    for(int i = 0; i<4; i++){
      if(targ->nb[i]!=&nullNode && (targ->nb[i]->bfsexplored)==UNEXPLORED) heap[heapEnd++] = (targ->nb[i]);
    }
  }
  while(heapEnd - heapStart>0){
    Node* currn = heap[heapStart++];
    currn->bfsexplored = EXPLORED;
    int valueLowest = 1000;
    for(int i = 0; i<4; i++){
      if(currn->nb[i]!=&nullNode && currn->nb[i]->bfsexplored==UNEXPLORED && currn->nb[i]->visited==UNVISITED){
        heap[heapEnd++] = currn->nb[i];
        currn->nb[i]->bfsexplored = EXPLORED;
      }
      if(currn->nb[i]!=&nullNode){
        if(dfme[currn->nb[i]->loc.x][currn->nb[i]->loc.y]<valueLowest){
          valueLowest = dfme[currn->nb[i]->loc.x][currn->nb[i]->loc.y];
        }
      }
    }
    dfme[currn->loc.x][currn->loc.y]=valueLowest==1000 ? valueLowest : valueLowest+1;
  }
  for(int i = 0; i<MAZESIZE; i++){
    for(int j = 0; j<MAZESIZE; j++){
      currMaze[i][j].bfsexplored = UNEXPLORED;
    }
  }
}

void bfs(Node *start, Node *end, Node **path, int *length){ // returns a path
  Node* heap[ARRLEN];
  int heapStart = 0;
  int heapEnd = 0;
  start->bfsScore = 0.0f;
  heap[heapEnd++] = start;
  Node* currn;
  bool found = false;
  while(heapEnd - heapStart > 0){
    currn = heap[heapStart++];
    if(currn == end){
      found = true;
      break;
    }
    currn->bfsexplored = EXPLORED;
    for(int i = 0; i< 4; i++){
      Node* nei = currn->nb[i];
      if(nei == &nullNode) continue;
      if(nei->bfsexplored == UNEXPLORED){
        nei->previous = currn;
        nei->bfsScore = currn->bfsScore+1;
        nei->bfsexplored = EXPLORED;
        heap[heapEnd++] = nei;
      }
    }
  }
  for(int x = 0; x < 16; x++){
    for(int y = 0; y < 16; y++){
      currMaze[x][y].bfsexplored = UNEXPLORED;
    }
  }
  heapStart = currn->bfsScore+1;
  heapEnd = currn->bfsScore+1;
  if(found){
    while(currn!=start){
      path[--heapStart] = currn;
      currn = currn->previous;
    }
    path[--heapStart] = currn;
    int t = heapEnd - heapStart;
    *length = t;
  }

}

Node* nextToMidSquare(){
  dfieldtarg(); dfieldme();
  
  int lowestscore = 1000;
  Node* lowestnode = &nullNode;
  for(int i = 0; i<ARRLEN; i++){
    Node* in = toVisit[i];
    if(!in->toVisit)continue;
    int score;
    if(dfme[in->loc.x][in->loc.y]==1000 || dftarg[in->loc.x][in->loc.y]==1000)score = 1000;
    else score = (dfme[in->loc.x][in->loc.y]+2)*(dftarg[in->loc.x][in->loc.y]); // decreased the weight of closeness to me
    if(score<lowestscore){
      lowestscore = score;
      lowestnode = in;
    }
  }
  Node* path[1000];
  int pathlen;
  bfs(currNode, lowestnode, path, &pathlen);
  return path[1];
}

void visitSquare(Direction dir, CellWalls cw, bool wallOnLeft, bool wallOnRight){
  Node* mine = currNode->nb[dir];
  if(wallOnLeft){
    mine->nb[(dir+1)%4]->resolveWall((dir+2)%4, true);
  }
  if(wallOnRight){
    mine->nb[(dir+3)%4]->resolveWall((dir+2)%4, true);
  }
  currNode = mine;
  if(mine->visited==VISITED)return;
  mine->visited = VISITED;
  if(mine->toVisit)mine->toVisit = false;
  // make da walls
  mine->resolveWall(dir, cw.front);
  mine->resolveWall((dir+1)%4, cw.left);
  mine->resolveWall((dir+3)%4, cw.right);
  for(int i = 0; i<4; i++){
    if(mine->nb[i]!=&nullNode){
      mine->nb[i]->toVisit = true;
    }
  }
}

int getDir(){
  Node* t = nextToMidSquare();
  for(int i = 0; i<4; i++){
    if(currNode->nb[i]==t)return i;
  }
  return 0;
}

Direction runRight(Direction dir, CellWalls cw){
  if(cw.front){
    return static_cast<Direction>((dir+2)%4);
  }else{
    return dir;
  }
}

// void run_maze_solve_cycle(){
//   // run_IR_cycle();
//   // SensorData g_dat = read_gyro();
//   // if(check_pause_state()) currPhase = ERROR;
  
//    STATE MACHINE 
//   switch(currPhase){
//     case IDLE:
//     break;
//     case MAZEERROR:
//     // runError();
//     break;
//     case SEARCH:
//     // runSearch();
//   }
// }

// void runIdle(){
//   // do nothing
//   return;
// }

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
// uint8_t ecounter = 0;
// Timer int_1;
// void runError(){
//   if(check_pause_state()){
//     ecounter = 0;
//     led.on(0,0,0,0,0,1);
//     m_left.setSpeed(0.0f); m_right.setSpeed(0.0f);
//   }else{
//     switch(ecounter){
//       case 0:
//       ecounter = 1;
//       led.off();
//       int_1.reset();
//       int_1.start();
//       break;
//       case 1:
//       if(int_1.read_ms() >= 1000)ecounter = 2;
//       break;
//       case 2:
//       led.on(0,0,0,0,1,0);
//       IRData ird = read_IR_async();
//       if(ird.l > START_IR_DIST && ird.r > START_IR_DIST)ecounter = 3;
//       break;
//       case 3:
//       led.on(0,0,0,1,0,0);
//       IRData ird = read_IR_async();
//       if(ird.l < f_thresh.l && ird.r > f_thresh.r){
//         ecounter = 4;
//         int_1.reset();
//         int_1.start();
//       }
//       break;
//       case 4:
//       led.off();
//       if(int_1.read_ms()>1000){
//         currPhase = lastStartPhase;
//         reset_walls();
//         reset_nav();
//       }
//       break;
//     }
//   }
// }