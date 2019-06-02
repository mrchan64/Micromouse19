/***********************************************************************
 *                                                                     *
 *   This FILE sets up the algorithm states in the Micromouse          *
 *                                                                     *
 *   Author: EDWARD CHEN                                               *
 *   Date: 5/12/2019                                                    *
 *   CPP FILE                                                          *
 *                                                                     *
 ***********************************************************************/

//#include "mbed.h"
#include "AlgoState.hpp"


//map init and generalization functions

void AlgoState::initMaze() {
  //add walls around entire map
  // for (int i = 0; i < MAZE_SIZE; i++) {
  //   for (int j = 0; j < MAZE_SIZE; j++) {
  //     maze[i][j] = new Node(i, j);
  //     maze[i][j]->northWall = true;
  //     maze[i][j]->southWall = true;
  //     maze[i][j]->eastWall = true;
  //     maze[i][j]->westWall = true;
    
  //     ff[i][j] = new Node(i, j);
  //   }
  // }

  // //init start and goal in maze
  // maze[15][0]->start = true;
  
  // //remove goal walls
  // maze[7][7]->eastWall = false;
  // maze[7][7]->southWall = false;
  // maze[8][7]->northWall = false;
  // maze[8][7]->eastWall = false;
  // maze[7][8]->westWall = false;
  // maze[7][8]->southWall = false;
  // maze[8][8]->westWall = false;
  // maze[8][8]->northWall = false;

  //set up ff map
  ff[15][0]->start = true;

  for (int i = 0; i < MAZE_SIZE; i++) {
    ff[0][i]->northWall = true;
    ff[MAZE_SIZE - 1][i]->southWall = true;
    ff[i][0]->westWall = true;
    ff[i][MAZE_SIZE - 1]->eastWall = true;
  }

  //make box for ff
  AlgoState::addWall(15, 0, EAST);
  AlgoState::setGoalTiles();
  AlgoState::resetGoal();
}

void AlgoState::setGoalTiles() {
  goalTiles->push_back(ff[7][7]);
  goalTiles->push_back(ff[8][7]);
  goalTiles->push_back(ff[7][8]);
  goalTiles->push_back(ff[8][8]);

  // goalTilesMaze->push_back(maze[7][7]);
  // goalTilesMaze->push_back(maze[8][7]);
  // goalTilesMaze->push_back(maze[7][8]);
  // goalTilesMaze->push_back(maze[8][8]);
}

// void AlgoState::generateMaze() {
//   AlgoState::initMaze();
//   stack<Node*> st;
//   vector<Node*>* neighbors = new vector<Node*>();
//   int unvisited = MAZE_SIZE * MAZE_SIZE;
//   srand(time(NULL));


//   Node* current = maze[15][0];
//   while (unvisited > 0) {
//     AlgoState::findNeighbors(current->r, current->c, neighbors);
//     if (neighbors->size() > 0) {
//       st.push(current);
//       unvisited--;

//       unsigned int pos = rand() % neighbors->size();
//       Node* neighbor = neighbors->at(pos);
//       if (current->c != neighbor->c) {
//         if (current->c < neighbor->c) {
//           current->eastWall = false;
//           neighbor->westWall = false;
//         }
//         else {
//           current->westWall = false;
//           neighbor->eastWall = false;
//         }
//       }
//       else {
//         if (current->r < neighbor->r) {
//           current->southWall = false;
//           neighbor->northWall = false;
//         }
//         else {
//           current->northWall = false;
//           neighbor->southWall = false;
//         }
//       }
//       current = neighbor;
//       current->visited = true;
//       neighbors->clear();
//     }
//     else if (!st.empty()) {
//       current = st.top();
//       st.pop();
//     }
//     else {
//       break;
//     }
//   }
//   AlgoState::removeRandomWalls();
//   //add box around start location
//   maze[15][0]->eastWall = true;
//   maze[15][1]->westWall = true;
//   delete neighbors;
// }

//maze functions
bool AlgoState::checkInMap(int r, int c) {
  if (r >= 0 && r < MAZE_SIZE &&
    c >= 0 && c < MAZE_SIZE) {
    return true;
  } 
  return false;
}

void AlgoState::addWall(int r, int c, Direction d) {
  if (!AlgoState::checkInMap(r, c)) { return; }

  if (d == NORTH) {
    ff[r][c]->northWall = true;
    if (AlgoState::checkInMap(r - 1, c)) {
      ff[r - 1][c]->southWall = true;
    }
  }
  else if (d == SOUTH) {
    ff[r][c]->southWall = true;
    if (AlgoState::checkInMap(r + 1, c)) {
      ff[r + 1][c]->northWall = true;
    }
  }
  else if (d == EAST) {
    ff[r][c]->eastWall = true;
    if (AlgoState::checkInMap(r, c + 1)) {
      ff[r][c + 1]->westWall = true;
    }
  } 
  else {
    ff[r][c]->westWall = true;
    if (AlgoState::checkInMap(r, c - 1)) {
      ff[r][c - 1]->eastWall = true;
    }
  }
}

// void AlgoState::findNeighbors(int r, int c, vector<Node*>* neighbors) {
//   if (AlgoState::checkInMap(r - 1, c) && !maze[r - 1][c]->visited) {
//     neighbors->push_back(maze[r - 1][c]);
//   }
//   if (AlgoState::checkInMap(r + 1, c) && !maze[r + 1][c]->visited) {
//     neighbors->push_back(maze[r + 1][c]);
//   }
//   if (AlgoState::checkInMap(r, c - 1) && !maze[r][c - 1]->visited) {
//     neighbors->push_back(maze[r][c - 1]);
//   }
//   if (AlgoState::checkInMap(r, c + 1) && !maze[r][c + 1]->visited) {
//     neighbors->push_back(maze[r][c + 1]);
//   }
// }

// void AlgoState::removeRandomWalls() {
//   srand(time(NULL));
//   int num_walls = 25;
//   int counter = 0;
//   while (counter < num_walls) {
//     int y = rand() % (MAZE_SIZE - 3) + 1;
//     int x = rand() % (MAZE_SIZE - 3) + 1;
//     if (maze[y][x]->northWall) {
//       maze[y][x]->northWall = false;
//       maze[y - 1][x]->southWall = false;
//       counter++;
//     }
//     else if (maze[y][x]->southWall) {
//       maze[y][x]->southWall = false;
//       maze[y + 1][x]->northWall = false;
//       counter++;
//     }
//     else if (maze[y][x]->eastWall) {
//       maze[y][x]->eastWall = false;
//       maze[y][x + 1]->westWall = false;
//       counter++;
//     }
//     else if (maze[y][x]->westWall) {
//       maze[y][x]->westWall = false;
//       maze[y][x - 1]->eastWall = false;
//       counter++;
//     }
//   }
// }


//print functions 

// void AlgoState::printMaze(vector<vector<Node*> > input_maze) {
//   for (int i = 0; i < MAZE_SIZE; i++) {
//     string firstLine = "";
//     string secondLine = "";
//     string thirdLine = "";
//     for (int j = 0; j < MAZE_SIZE; j++) {
//       if (input_maze[i][j]->northWall) {
//         firstLine += WALL + WALL;
//       }
//       else {
//         if (input_maze[i][j]->westWall) {
//           firstLine += WALL + PATH;
//         }
//         else {
//           firstLine += PATH + PATH;
//         }
//         //if (maze[i][j].eastWall) {
//         //  firstLine += WALL;
//         //}
//         //else {
//         //  firstLine += PATH;
//         //}
//       }

//       string type = PATH;
//       if (input_maze[i][j]->criticalPath) {
//         type = CRITICAL_PATH;
//       }
//       if (input_maze[i][j]->start) {
//         type = "S";
//       }
//       else if (input_maze[i][j]->goal) {
//         type = "G";
//       }
//       if (input_maze[i][j]->current) {
//         type = CURR;
//       }

//       if (input_maze[i][j]->westWall) {
//         secondLine += WALL+type;
//       } 
//       else {
//         secondLine += PATH+type;
//       }
//       //if (maze[i][j].eastWall) {
//       //  secondLine += WALL;
//       //}
//       //else {
//       //  secondLine += PATH;
//       //}

//       if (input_maze[i][j]->southWall) {
//         thirdLine += WALL + WALL;
//       }
//       else {
//         if (input_maze[i][j]->westWall) {
//           thirdLine += WALL + PATH;
//         }
//         else {
//           thirdLine += PATH + PATH;
//         }
//         //if (maze[i][j].eastWall) {
//         //  thirdLine += WALL;
//         //}
//         //else {
//         //  thirdLine += PATH;
//         //}
//       }
//     }
//     if (input_maze[i][MAZE_SIZE-1]->eastWall) {
//       firstLine += WALL;
//       secondLine += WALL;
//       thirdLine += WALL;
//     }
//     else {
//       firstLine += PATH;
//       secondLine += PATH;
//       thirdLine += PATH;
//     }

//     cout << firstLine << endl;
//     cout << secondLine << endl;
//     //cout << thirdLine << endl;
//     if (i == MAZE_SIZE - 1) {
//       cout << thirdLine << endl;
//     }
//   }
// }

// void AlgoState::printDist() {
//   for (int i = 0; i < MAZE_SIZE; i++) {
//     for (int j = 0; j < MAZE_SIZE; j++) {
//       if (dist[i][j] >= 10) {
//         cout << dist[i][j] << " ";
//       }
//       else {
//         cout << dist[i][j] << "  ";
//       }
//     }
//     cout << endl;
//   }
//   cout << endl;
// }

// void AlgoState::printVisited() {
//   for (int i = 0; i < MAZE_SIZE; i++) {
//     for (int j = 0; j < MAZE_SIZE; j++) {
//       cout << ff[i][j]->visited;
//     }
//     cout << endl;
//   }
//   cout << endl;
// }




//floodfill

//bfs on current location to the closest node on optimal path that is unvisited

void AlgoState::floodFill() {
  AlgoState::reachGoal(goalTiles);
  // cout << "REACHED GOAL!" << endl;

  vector<Node*>* unvisited = new vector<Node*>();
  //check unvisited
  bool optimalPath = AlgoState::checkOptimalPath(ff, unvisited);
  // cout << "ANY UNVISTED NODES? " << !optimalPath << " " << unvisited->size() << endl;
  while (!optimalPath) {
    // cout << "NO OPTIMAL PATH YET " << endl;
    //bfs to unvisited nodes
    AlgoState::bfsToNode(unvisited);
    //recheck until no more updates
    unvisited->clear();
    AlgoState::clearGoal(ff);
    AlgoState::resetGoal();
    AlgoState::reweightDistance(ff, goalTiles);
    optimalPath = AlgoState::checkOptimalPath(ff, unvisited);
  }
  unvisited->clear();

  //go back to start 
  unvisited->push_back(ff[15][0]);
  AlgoState::bfsToNode(unvisited);

  //reset and draw
  unvisited->clear();
  AlgoState::clearGoal(ff);
  AlgoState::resetGoal();
  AlgoState::reweightDistance(ff, goalTiles);
  optimalPath = AlgoState::checkOptimalPath(ff, unvisited);
  delete unvisited;
}

void AlgoState::bfsToNode(vector<Node*>* targets) {
  AlgoState::clearGoal(ff);
  AlgoState::setGoal(ff, targets);
  AlgoState::reweightDistance(ff, targets);
  // cout << "GOALS SET" << endl;
  // printMaze(ff);
  // printDist();
  AlgoState::reachGoal(targets);
}

void AlgoState::reachGoal(vector<Node*>* targets/*, CellWalls cw*/) { // call this
  Node* current = ff[current_r][current_c];
  bool notReachedGoal = true;
  while (notReachedGoal) {
    if (current->goal) {
      notReachedGoal = false;
    }
    current->current = true;
    current->visited = true;

    //add walls from maze
    //REPLACE WITH WALL DETECTION CODE HERE
    //NEED TO PLACE WALL RELATIVE TO FACING

    // if(cw.front){
    //   AlgoState::addWall(current_r, current_c, facing);
    // }
    // if(cw.left){
    //   AlgoState::addWall(current_r, current_c, (facing+3)%4);
    // }
    // if(cw.right){
    //   AlgoState::addWall(current_r, current_c, (facing+1)%4);
    // }
    
    //translate direction given current facing
    // if (maze[current_r][current_c]->northWall) {
    //   Direction nextDir = AlgoState::translateWall(NORTH);
    //   AlgoState::addWall(current_r, current_c, nextDir);
    // }
    // if (maze[current_r][current_c]->southWall) {
    //   Direction nextDir = AlgoState::translateWall(SOUTH);
    //   AlgoState::addWall(current_r, current_c, nextDir);
    // }
    // if (maze[current_r][current_c]->eastWall) {
    //   Direction nextDir = AlgoState::translateWall(EAST);
    //   AlgoState::addWall(current_r, current_c, nextDir);
    // }
    // if (maze[current_r][current_c]->westWall) {
    //   Direction nextDir = AlgoState::translateWall(WEST);
    //   AlgoState::addWall(current_r, current_c, nextDir);
    // }

    //recalculate distance after adding walls
    led.l2 = 1;
    AlgoState::reweightDistance(ff, targets);
    //AlgoState::printMaze(ff);
    Node* next = AlgoState::nextBestMove();
    led.l2 = 0;
    Direction nextDir = AlgoState::dirNextMove(current, next);

    //calculate how to turn
    if (nextDir != facing) {
      //call turning code here
      //facing = nextDir;
    }
    current = next;
  }
}

Direction AlgoState::nextBestMoveToGoal(CellWalls cw){
  Node* current = ff[current_r][current_c];
  // if (current->goal) {
  //   notReachedGoal = false;
  // }
  current->current = true;
  current->visited = true;

  if(cw.front){
    AlgoState::addWall(current_r, current_c, facing);
  }
  if(cw.left){
    AlgoState::addWall(current_r, current_c, static_cast<Direction>((facing+3)%4));
  }
  if(cw.right){
    AlgoState::addWall(current_r, current_c, static_cast<Direction>((facing+1)%4));
  }

  AlgoState::reweightDistance(ff, goalTiles);
  Node* next = AlgoState::nextBestMove();
  Direction nextDir = AlgoState::dirNextMove(current, next);
  current = next;
  return nextDir;

}

Direction AlgoState::translateWall(Direction wall) {
  if (facing == NORTH) {
    return wall;
  }
  else if (facing == SOUTH) {
    //everything is backwards
    if (wall == NORTH) {
      return SOUTH;
    }
    else if (wall == WEST) {
      return EAST;
    }
    else if (wall == EAST) {
      return WEST;
    }
    else {
      return NORTH;
    }
  }
  else if (facing == EAST) {
    if (wall == NORTH) {
      return EAST;
    }
    else if (wall == SOUTH) {
      return WEST;
    }
    else if (wall == WEST) {
      return NORTH;
    }
    else {
      return SOUTH;
    }
  }
  else {
    if (wall == NORTH) {
      return WEST;
    }
    else if (wall == SOUTH) {
      return EAST;
    }
    else if (wall == WEST) {
      return SOUTH;
    }
    else {
      return EAST;
    }
  }
}

Direction AlgoState::dirNextMove(Node* current, Node* next) {
  if (current->r != next->r) {
    if (current->r > next->r) {
      return NORTH;
    }
    else {
      return SOUTH;
    }
  }
  else {
    if (current->c > next->c) {
      return WEST;
    }
    else {
      return EAST;
    }
  }
}



Node* AlgoState::nextBestMove() {
  vector<Node*>* neighbors = new vector<Node*>();
  AlgoState::findNeighborsWW(current_r, current_c, neighbors, ff);
  bool idealPathFound = false;
  Node* next;
  int nextMin = 100;
  Node* potentNext;
  for (unsigned int i = 0; i < neighbors->size(); i++) {
    if (dist[neighbors->at(i)->r][neighbors->at(i)->c] ==
      dist[current_r][current_c] - 1 && neighbors->at(i)->visited) {
      idealPathFound = true;
      next = neighbors->at(i);
    }
    if (dist[neighbors->at(i)->r][neighbors->at(i)->c] < nextMin - 1) {
      nextMin = dist[neighbors->at(i)->r][neighbors->at(i)->c] + 1;
      potentNext = neighbors->at(i);
    }
  }

  delete neighbors;

  ff[current_r][current_c]->current = false;
  if (idealPathFound) {
    current_r = next->r;
    current_c = next->c;
    return next;
  }
  else {
    //find neighbor with lowest weight
    current_r = potentNext->r;
    current_c = potentNext->c;
    return potentNext;
  }
}

void AlgoState::setGoal(vector<vector<Node*> > input_maze, vector<Node*>* targets) {
  for (unsigned int i = 0; i < targets->size(); i++) {
    int r = targets->at(i)->r;
    int c = targets->at(i)->c;
    input_maze[r][c]->goal = true;
  }
}

void AlgoState::clearGoal(vector<vector<Node*> > input_maze) {
  for (int i = 0; i < MAZE_SIZE; i++) {
    for (int j = 0; j < MAZE_SIZE; j++) {
      input_maze[i][j]->goal = false;
    }
  }
}

void AlgoState::resetGoal() {
  for (unsigned int i = 0; i < goalTiles->size(); i++) {
    goalTiles->at(i)->goal = true;
  }
}

void AlgoState::resetRVisited(vector<vector<Node*> > input_maze) {
  for (int i = 0; i < MAZE_SIZE; i++) {
    for (int j = 0; j < MAZE_SIZE; j++) {
      input_maze[i][j]->rvisited = false;
      input_maze[i][j]->criticalPath = false;
    }
  }
}

void AlgoState::reweightDistance(vector<vector<Node*> > input_maze, vector<Node*>* targets) {
  AlgoState::resetRVisited(input_maze);

  int weight = 0;
  vector<Node*>* neighbors = new vector<Node*>();

  queue<Node*> queue;

  for (unsigned int i = 0; i < targets->size(); i++) {
    queue.push(targets->at(i));
    targets->at(i)->rvisited = true;
  }

  while (!queue.empty()) {
    int length = queue.size();
    for (int i = 0; i < length; i++) {
      Node* current = queue.front();
      current->rvisited = true;
      queue.pop();
      dist[current->r][current->c] = weight;
      AlgoState::findNeighborsRWW(current->r, current->c, neighbors, input_maze);
      for (unsigned int j = 0; j < neighbors->size(); j++) {
        queue.push(neighbors->at(j));
        neighbors->at(j)->rvisited = true;
      }
      neighbors->clear();
    }
    neighbors->clear();
    weight++;
  }
  delete neighbors;
  // AlgoState::printDist();
}

bool AlgoState::checkOptimalPath(vector<vector<Node*> > input_maze, vector<Node*>* unvisited) {
  AlgoState::resetRVisited(input_maze);
  vector<Node*>* neighbors = new vector<Node*>();

  bool foundOptimalPath = true;

  Node* curr = input_maze[15][0];
  while (!curr->goal) {
    curr->criticalPath = true;
    AlgoState::findNeighborsRWW(curr->r, curr->c, neighbors, input_maze);

    bool unvisitedNode = true;
    Node* potentNext;
    for (unsigned int i = 0; i < neighbors->size(); i++) {
      if (dist[neighbors->at(i)->r][neighbors->at(i)->c] ==
        dist[curr->r][curr->c] - 1 && neighbors->at(i)->visited) {
        curr = neighbors->at(i);
        unvisitedNode = false;
      }
      if (dist[neighbors->at(i)->r][neighbors->at(i)->c] ==
        dist[curr->r][curr->c] - 1) {
        potentNext = neighbors->at(i);
      }
    }
    if (unvisitedNode) {
      foundOptimalPath = false;
      unvisited->push_back(potentNext);
      curr = potentNext;
    }
    neighbors->clear();
  }
  delete neighbors;
  return foundOptimalPath;
}

void AlgoState::findNeighborsWW(int r, int c, vector<Node*>* neighbors, vector<vector<Node*> > input_maze) {
  if (AlgoState::checkInMap(r - 1, c) && !input_maze[r][c]->northWall) {
    neighbors->push_back(input_maze[r - 1][c]);
  }
  if (AlgoState::checkInMap(r + 1, c) && !input_maze[r][c]->southWall) {
    neighbors->push_back(input_maze[r + 1][c]);
  }
  if (AlgoState::checkInMap(r, c - 1) && !input_maze[r][c]->westWall) {
    neighbors->push_back(input_maze[r][c - 1]);
  }
  if (AlgoState::checkInMap(r, c + 1) && !input_maze[r][c]->eastWall) {
    neighbors->push_back(input_maze[r][c + 1]);
  }
}

void AlgoState::findNeighborsRWW(int r, int c, vector<Node*>* neighbors, vector<vector<Node*> > input_maze) {
  if (AlgoState::checkInMap(r - 1, c) && !input_maze[r - 1][c]->rvisited && !input_maze[r][c]->northWall) {
    neighbors->push_back(input_maze[r - 1][c]);
  }
  if (AlgoState::checkInMap(r + 1, c) && !input_maze[r + 1][c]->rvisited && !input_maze[r][c]->southWall) {
    neighbors->push_back(input_maze[r + 1][c]);
  }
  if (AlgoState::checkInMap(r, c - 1) && !input_maze[r][c - 1]->rvisited && !input_maze[r][c]->westWall) {
    neighbors->push_back(input_maze[r][c - 1]);
  }
  if (AlgoState::checkInMap(r, c + 1) && !input_maze[r][c + 1]->rvisited && !input_maze[r][c]->eastWall) {
    neighbors->push_back(input_maze[r][c + 1]);
  }
}

/*int main() {
  AlgoState::generateMaze();
  AlgoState::printMaze(maze);
  AlgoState::reweightDistance(maze, goalTilesMaze);
  AlgoState::printDist();
  AlgoState::reweightDistance(ff, goalTiles);
  AlgoState::printDist();
  AlgoState::printMaze(ff);

  cout << endl;
  AlgoState::floodFill();
  cout << "FINSIHED FLOOD FILL" << endl;
  AlgoState::printMaze(ff);
  AlgoState::printMaze(maze);
  AlgoState::printDist();
  return 0;
}*/

// int get_r(){
//   return current_r;
// }

// int get_c(){
//   return current_c;
// }

void set_facing(Direction dir){
  facing = dir;
}