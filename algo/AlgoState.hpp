/***********************************************************************
 *                                                                     *
 *   This FILE sets up the algorithm states in the Micromouse          *
 *                                                                     *
 *   Author: EDWARD CHEN                                               *
 *   Date: 5/12/2019                                                    *
 *   CPP FILE                                                          *
 *                                                                     *
 ***********************************************************************/

#ifndef ALGO_STATE_H
#define ALGO_STATE_H

#include <vector>
#include <iostream>
#include <stack>
#include <queue>
#include <utility>
// #include <string>
// #include <stdlib.h>  
// #include <time.h>


using namespace std;

enum dir
{
  NORTH = 0,
  SOUTH = 1,
  EAST = 2,
  WEST = 3
};

struct Node {
  bool northWall = false;
  bool southWall = false;
  bool eastWall = false;
  bool westWall = false;

  bool start = false;
  bool goal = false;
  bool visited = false;
  bool rvisited = false;

  bool current = false;
  bool criticalPath = false;

  int r, c;

  Node(int ar, int ac) : r(ar), c(ac) {}
  ~Node() {}
};

class AlgoState {
public: 
  //map init and generalization functions
  static void initMaze();
  // static void generateMaze();

  //maze functions
  static bool checkInMap(int r, int c);
  static void addWall(int r, int c, dir dir);
  static void findNeighbors(int r, int c, vector<Node*>* neighbors);
  // static void removeRandomWalls();

  //print functions 
  // static void printMaze(vector<vector<Node*>> input_maze); 
  // static void printDist();
  // static void printVisited();
  
  //floodfill
  static void floodFill();
  static void reachGoal(vector<Node*>* targets);
  static void bfsToNode(vector<Node*>* targets);
  static void setGoal(vector<vector<Node*>> input_maze, vector<Node*>* targets);
  static void clearGoal(vector<vector<Node*>> input_maze);
  static void resetGoal();
  static void resetRVisited(vector<vector<Node*>> input_maze);
  static void reweightDistance(vector<vector<Node*>> input_maze, vector<Node*>* targets);
  static bool checkOptimalPath(vector<vector<Node*>> input_maze, vector<Node*>* unvisited);
  static void findNeighborsWW(int r, int c, vector<Node*>* neighbors, vector<vector<Node*>> input_maze);
  static void findNeighborsRWW(int r, int c, vector<Node*>* neighbors, vector<vector<Node*>> input_maze);

};

#endif