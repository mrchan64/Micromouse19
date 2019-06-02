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

#include "vector"
#include <iostream>
#include <stack>
#include <queue>
#include <utility>
#include <stdlib.h>  
#include <time.h>
#include "WallState.hpp"
#include "../lib/MiscIO.hpp"

using namespace std;

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

enum Direction
{
  NORTH = 0,
  WEST = 1,
  SOUTH = 2,
  EAST = 3,
};

const string WALL = "-";
const string PATH = ".";
const string CRITICAL_PATH = "^";
const string CURR = "%";

const int MAZE_SIZE = 16;
const int GOAL = 1;
const int START = 2;

static Direction facing = NORTH;

static int current_r = 15;
static int current_c = 0;

//maze generation
static vector<vector<Node*> > maze(MAZE_SIZE, vector<Node *>(MAZE_SIZE));

//flood fill algorithm
static vector<vector<Node*> > ff(MAZE_SIZE, vector<Node *>(MAZE_SIZE));
static vector<vector<int> > dist(MAZE_SIZE, vector<int>(MAZE_SIZE));
static vector<Node*>* goalTiles = new vector<Node*>();
// static vector<Node*>* goalTilesMaze = new vector<Node*>();

// static int get_r();
// static int get_c();
// static void set_facing(Direction dir);





class AlgoState {
public: 
  //map init and generalization functions
  static void initMaze();
  static void setGoalTiles();
  static void generateMaze();

  //maze functions
  static bool checkInMap(int r, int c);
  static void addWall(int r, int c, Direction dir);
  static void findNeighbors(int r, int c, vector<Node*>* neighbors);
  static void removeRandomWalls();

  //print functions 
  static void printMaze(vector<vector<Node*> > input_maze); 
  static void printDist();
  static void printVisited();
  
  //floodfill
  static void floodFill();
  static void reachGoal(vector<Node*>* targets);
  static Node* nextBestMove();
  static Direction dirNextMove(Node* current, Node* next);
  static Direction translateWall(Direction wall);
  static void bfsToNode(vector<Node*>* targets);
  static void setGoal(vector<vector<Node*> > input_maze, vector<Node*>* targets);
  static void clearGoal(vector<vector<Node*> > input_maze);
  static void resetGoal();
  static void resetRVisited(vector<vector<Node*> > input_maze);
  static void reweightDistance(vector<vector<Node*> > input_maze, vector<Node*>* targets);
  static bool checkOptimalPath(vector<vector<Node*> > input_maze, vector<Node*>* unvisited);
  static void findNeighborsWW(int r, int c, vector<Node*>* neighbors, vector<vector<Node*> > input_maze);
  static void findNeighborsRWW(int r, int c, vector<Node*>* neighbors, vector<vector<Node*> > input_maze);
  static Direction nextBestMoveToGoal(CellWalls cw);

};

#endif