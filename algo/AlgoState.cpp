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

const int MAZE_SIZE = 16;
const int GOAL = 1;
const int START = 2;
// const string WALL = "^";
// const string PATH = " ";
// const string CURR = "+";
// const string CRITICAL_PATH = "-";

int current_r = 15;
int current_c = 0;

//maze generation
static vector<vector<Node*>> maze(MAZE_SIZE, vector<Node *>(MAZE_SIZE));

//flood fill algorithm
static vector<vector<Node*>> ff(MAZE_SIZE, vector<Node *>(MAZE_SIZE));
static vector<vector<int>> dist(MAZE_SIZE, vector<int>(MAZE_SIZE));
static vector<Node*>* goalTiles = new vector<Node*>();
static vector<Node*>* goalTilesMaze = new vector<Node*>();



//map init and generalization functions

void AlgoState::initMaze() {
  //add walls around entire map
  for (auto i = 0; i < MAZE_SIZE; i++) {
    for (auto j = 0; j < MAZE_SIZE; j++) {
      maze[i][j] = new Node(i, j);
      maze[i][j]->northWall = true;
      maze[i][j]->southWall = true;
      maze[i][j]->eastWall = true;
      maze[i][j]->westWall = true;
    
      ff[i][j] = new Node(i, j);
    }
  }

  //init start and goal in maze
  maze[15][0]->start = true;
  maze[7][7]->goal = true;
  maze[8][7]->goal = true;
  maze[7][8]->goal = true;
  maze[8][8]->goal = true;
  
  //remove goal walls
  maze[7][7]->eastWall = false;
  maze[7][7]->southWall = false;
  maze[8][7]->northWall = false;
  maze[8][7]->eastWall = false;
  maze[7][8]->westWall = false;
  maze[7][8]->southWall = false;
  maze[8][8]->westWall = false;
  maze[8][8]->northWall = false;

  //set up ff map
  ff[15][0]->start = true;
  ff[7][7]->goal = true;
  ff[8][7]->goal = true;
  ff[7][8]->goal = true;
  ff[8][8]->goal = true;

  for (auto i = 0; i < MAZE_SIZE; i++) {
    ff[0][i]->northWall = true;
    ff[MAZE_SIZE - 1][i]->southWall = true;
    ff[i][0]->westWall = true;
    ff[i][MAZE_SIZE - 1]->eastWall = true;
  }

  //make box for ff
  AlgoState::addWall(15, 0, EAST);

  goalTiles->push_back(ff[7][7]);
  goalTiles->push_back(ff[8][7]);
  goalTiles->push_back(ff[7][8]);
  goalTiles->push_back(ff[8][8]);

  goalTilesMaze->push_back(maze[7][7]);
  goalTilesMaze->push_back(maze[8][7]);
  goalTilesMaze->push_back(maze[7][8]);
  goalTilesMaze->push_back(maze[8][8]);
}

void AlgoState::generateMaze() {
  AlgoState::initMaze();
  stack<Node*> st;
  vector<Node*>* neighbors = new vector<Node*>();
  int unvisited = MAZE_SIZE * MAZE_SIZE;
  srand(time(NULL));


  Node* current = maze[15][0];
  while (unvisited > 0) {
    AlgoState::findNeighbors(current->r, current->c, neighbors);
    if (neighbors->size() > 0) {
      st.push(current);
      unvisited--;

      int pos = rand() % neighbors->size();
      Node* neighbor = neighbors->at(pos);
      if (current->c != neighbor->c) {
        if (current->c < neighbor->c) {
          current->eastWall = false;
          neighbor->westWall = false;
        }
        else {
          current->westWall = false;
          neighbor->eastWall = false;
        }
      }
      else {
        if (current->r < neighbor->r) {
          current->southWall = false;
          neighbor->northWall = false;
        }
        else {
          current->northWall = false;
          neighbor->southWall = false;
        }
      }
      current = neighbor;
      current->visited = true;
      neighbors->clear();
    }
    else if (!st.empty()) {
      current = st.top();
      st.pop();
    }
    else {
      break;
    }
  }
  AlgoState::removeRandomWalls();
  //add box around start location
  maze[15][0]->eastWall = true;
  maze[15][1]->westWall = true;
}

//maze functions
bool AlgoState::checkInMap(int r, int c) {
  if (r >= 0 && r < MAZE_SIZE &&
    c >= 0 && c < MAZE_SIZE) {
    return true;
  } 
  return false;
}

void AlgoState::addWall(int r, int c, dir d) {
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

void AlgoState::findNeighbors(int r, int c, vector<Node*>* neighbors) {
  if (AlgoState::checkInMap(r - 1, c) && !maze[r - 1][c]->visited) {
    neighbors->push_back(maze[r - 1][c]);
  }
  if (AlgoState::checkInMap(r + 1, c) && !maze[r + 1][c]->visited) {
    neighbors->push_back(maze[r + 1][c]);
  }
  if (AlgoState::checkInMap(r, c - 1) && !maze[r][c - 1]->visited) {
    neighbors->push_back(maze[r][c - 1]);
  }
  if (AlgoState::checkInMap(r, c + 1) && !maze[r][c + 1]->visited) {
    neighbors->push_back(maze[r][c + 1]);
  }
}

void AlgoState::removeRandomWalls() {
  srand(time(NULL));
  int num_walls = 25;
  int counter = 0;
  while (counter < num_walls) {
    int y = rand() % (MAZE_SIZE - 3) + 1;
    int x = rand() % (MAZE_SIZE - 3) + 1;
    if (maze[y][x]->northWall) {
      maze[y][x]->northWall = false;
      maze[y - 1][x]->southWall = false;
      counter++;
    }
    else if (maze[y][x]->southWall) {
      maze[y][x]->southWall = false;
      maze[y + 1][x]->northWall = false;
      counter++;
    }
    else if (maze[y][x]->eastWall) {
      maze[y][x]->eastWall = false;
      maze[y][x + 1]->westWall = false;
      counter++;
    }
    else if (maze[y][x]->westWall) {
      maze[y][x]->westWall = false;
      maze[y][x - 1]->eastWall = false;
      counter++;
    }
  }
}


//print functions 

void AlgoState::printMaze(vector<vector<Node*>> input_maze) {
  for (auto i = 0; i < MAZE_SIZE; i++) {
    string firstLine = "";
    string secondLine = "";
    string thirdLine = "";
    for (auto j = 0; j < MAZE_SIZE; j++) {
      if (input_maze[i][j]->northWall) {
        firstLine += WALL + WALL;
      }
      else {
        if (input_maze[i][j]->westWall) {
          firstLine += WALL + PATH;
        }
        else {
          firstLine += PATH + PATH;
        }
        //if (maze[i][j].eastWall) {
        //  firstLine += WALL;
        //}
        //else {
        //  firstLine += PATH;
        //}
      }

      string type = PATH;
      if (input_maze[i][j]->criticalPath) {
        type = CRITICAL_PATH;
      }
      if (input_maze[i][j]->start) {
        type = "S";
      }
      else if (input_maze[i][j]->goal) {
        type = "G";
      }
      else if (input_maze[i][j]->current) {
        type = CURR;
      }

      if (input_maze[i][j]->westWall) {
        secondLine += WALL+type;
      } 
      else {
        secondLine += PATH+type;
      }
      //if (maze[i][j].eastWall) {
      //  secondLine += WALL;
      //}
      //else {
      //  secondLine += PATH;
      //}

      if (input_maze[i][j]->southWall) {
        thirdLine += WALL + WALL;
      }
      else {
        if (input_maze[i][j]->westWall) {
          thirdLine += WALL + PATH;
        }
        else {
          thirdLine += PATH + PATH;
        }
        //if (maze[i][j].eastWall) {
        //  thirdLine += WALL;
        //}
        //else {
        //  thirdLine += PATH;
        //}
      }
    }
    if (input_maze[i][MAZE_SIZE-1]->eastWall) {
      firstLine += WALL;
      secondLine += WALL;
      thirdLine += WALL;
    }
    else {
      firstLine += PATH;
      secondLine += PATH;
      thirdLine += PATH;
    }

    cout << firstLine << endl;
    cout << secondLine << endl;
    //cout << thirdLine << endl;
    if (i == MAZE_SIZE - 1) {
      cout << thirdLine << endl;
    }
  }
}

void AlgoState::printDist() {
  for (auto i = 0; i < MAZE_SIZE; i++) {
    for (auto j = 0; j < MAZE_SIZE; j++) {
      if (dist[i][j] >= 10) {
        cout << dist[i][j] << " ";
      }
      else {
        cout << dist[i][j] << "  ";
      }
    }
    cout << endl;
  }
  cout << endl;
}

void AlgoState::printVisited() {
  for (auto i = 0; i < MAZE_SIZE; i++) {
    for (auto j = 0; j < MAZE_SIZE; j++) {
      cout << ff[i][j]->visited;
    }
    cout << endl;
  }
  cout << endl;
}




//floodfill

//bfs on current location to the closest node on optimal path that is unvisited

void AlgoState::floodFill() {
  AlgoState::reachGoal(goalTiles);
  cout << "REACHED GOAL!" << endl;

  vector<Node*>* unvisited = new vector<Node*>();
  //check unvisited
  bool optimalPath = AlgoState::checkOptimalPath(ff, unvisited);
  cout << "ANY UNVISTED NODES? " << !optimalPath << " " << unvisited->size() << endl;
  while (!optimalPath) {
    cout << "NO OPTIMAL PATH YET " << endl;
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
  cout << "CLEAR EVERYTHING AND RESET GOALS" << endl;
  AlgoState::clearGoal(ff);
  AlgoState::resetGoal();
  printMaze(ff);
  AlgoState::reweightDistance(ff, goalTiles);
  optimalPath = AlgoState::checkOptimalPath(ff, unvisited);
  cout << "CHECK OPTIMAL PATH AGAIN" << endl;
}

void AlgoState::bfsToNode(vector<Node*>* targets) {
  AlgoState::clearGoal(ff);
  AlgoState::setGoal(ff, targets);
  AlgoState::reweightDistance(ff, targets);
  cout << "GOALS SET" << endl;
  printMaze(ff);
  printDist();
  AlgoState::reachGoal(targets);
}

void AlgoState::reachGoal(vector<Node*>* targets) {
  Node* current = ff[current_r][current_c];
  vector<Node*>* neighbors = new vector<Node*>();
  bool notReachedGoal = true;
  while (notReachedGoal) {
    if (current->goal) {
      notReachedGoal = false;
    }
    current->current = true;
    current->visited = true;

    //add walls from maze
    int r = current->r;
    int c = current->c;
    if (maze[r][c]->northWall) {
      AlgoState::addWall(r, c, NORTH);
    }
    if (maze[r][c]->southWall) {
      AlgoState::addWall(r, c, SOUTH);
    }
    if (maze[r][c]->eastWall) {
      AlgoState::addWall(r, c, EAST);
    }
    if (maze[r][c]->westWall) {
      AlgoState::addWall(r, c, WEST);
    }

    //recalculate distance after adding walls
    AlgoState::reweightDistance(ff, targets);
    AlgoState::findNeighborsWW(r, c, neighbors, ff);
    bool idealPathFound = false;
    Node* next;
    int nextMin = 100;
    Node* potentNext;
    for (auto i = 0; i < neighbors->size(); i++) {
      if (dist[neighbors->at(i)->r][neighbors->at(i)->c] ==
        dist[r][c] - 1 && neighbors->at(i)->visited) {
        idealPathFound = true;
        next = neighbors->at(i);
      }
      if (dist[neighbors->at(i)->r][neighbors->at(i)->c] < nextMin - 1) {
        nextMin = dist[neighbors->at(i)->r][neighbors->at(i)->c] + 1;
        potentNext = neighbors->at(i);
      }
    }
    AlgoState::printMaze(ff);
    //AlgoState::printDist();
    //AlgoState::printMaze(maze);
    neighbors->clear();

    current->current = false;
    if (idealPathFound) {
      current = next;
      current_r = next->r;
      current_c = next->c;
    }
    else {
      //find neighbor with lowest weight
      current = potentNext;
      current_r = potentNext->r;
      current_c = potentNext->c;
    }
    
    AlgoState::printVisited();
  }
}

void AlgoState::setGoal(vector<vector<Node*>> input_maze, vector<Node*>* targets) {
  for (auto i = 0; i < targets->size(); i++) {
    int r = targets->at(i)->r;
    int c = targets->at(i)->c;
    input_maze[r][c]->goal = true;
  }
}

void AlgoState::clearGoal(vector<vector<Node*>> input_maze) {
  for (auto i = 0; i < MAZE_SIZE; i++) {
    for (auto j = 0; j < MAZE_SIZE; j++) {
      input_maze[i][j]->goal = false;
    }
  }
}

void AlgoState::resetGoal() {
  ff[7][7]->goal = true;
  ff[8][7]->goal = true;
  ff[7][8]->goal = true;
  ff[8][8]->goal = true;
}

void AlgoState::resetRVisited(vector<vector<Node*>> input_maze) {
  for (auto i = 0; i < MAZE_SIZE; i++) {
    for (auto j = 0; j < MAZE_SIZE; j++) {
      input_maze[i][j]->rvisited = false;
      input_maze[i][j]->criticalPath = false;
    }
  }
}

void AlgoState::reweightDistance(vector<vector<Node*>> input_maze, vector<Node*>* targets) {
  AlgoState::resetRVisited(input_maze);

  int weight = 0;
  vector<Node*>* neighbors = new vector<Node*>();

  queue<Node*> queue;

  for (int i = 0; i < targets->size(); i++) {
    queue.push(targets->at(i));
    targets->at(i)->rvisited = true;
  }

  while (!queue.empty()) {
    int length = queue.size();
    for (auto i = 0; i < length; i++) {
      Node* current = queue.front();
      current->rvisited = true;
      queue.pop();
      dist[current->r][current->c] = weight;
      AlgoState::findNeighborsRWW(current->r, current->c, neighbors, input_maze);
      for (auto j = 0; j < neighbors->size(); j++) {
        queue.push(neighbors->at(j));
        neighbors->at(j)->rvisited = true;
      }
      neighbors->clear();
    }
    neighbors->clear();
    weight++;
  }
  AlgoState::printDist();
}

bool AlgoState::checkOptimalPath(vector<vector<Node*>> input_maze, vector<Node*>* unvisited) {
  AlgoState::resetRVisited(input_maze);
  vector<Node*>* neighbors = new vector<Node*>();

  bool foundOptimalPath = true;

  Node* curr = input_maze[15][0];
  while (!curr->goal) {
    curr->criticalPath = true;
    AlgoState::findNeighborsRWW(curr->r, curr->c, neighbors, input_maze);

    bool unvisitedNode = true;
    Node* potentNext;
    for (auto i = 0; i < neighbors->size(); i++) {
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
  return foundOptimalPath;
}

void AlgoState::findNeighborsWW(int r, int c, vector<Node*>* neighbors, vector<vector<Node*>> input_maze) {
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

void AlgoState::findNeighborsRWW(int r, int c, vector<Node*>* neighbors, vector<vector<Node*>> input_maze) {
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

int main() {
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
}