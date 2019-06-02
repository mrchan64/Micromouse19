/********************************************************************
 *                                                                  *
 *   This FILE manages the individual node data of the Micromouse   *
 *                                                                  *
 ********************************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "MazeNode.hpp"

// METHODS ---------------------------------------------------
Node::Node(){
  for(int i = 0; i< 4; i++){
    this->nbwalls[i] = UNSURE;
    this->nb[i] = NULL;
  }
  this->visited = UNVISITED;
  this->bfsexplored = UNEXPLORED;
  this->bfsScore = 0;
  this->toVisit = false;
}

Node::Node(int x_pos, int y_pos){
  this->loc.x = x_pos;
  this->loc.y = y_pos;
  for(int i = 0; i< 4; i++){
    this->nbwalls[i] = UNSURE;
    this->nb[i] = NULL;
  }
  this->visited = UNVISITED;
  this->bfsexplored = UNEXPLORED;
  this->bfsScore = 0;
  this->toVisit = false;
}

void Node::setXY(int x_pos, int y_pos){
  this->loc.x = x_pos;
  this->loc.y = y_pos;
}

void Node::resolveWall(int dir, bool hasWall){
  if(this->nbwalls[dir] != UNSURE)return;
  if(hasWall){
    this->nbwalls[dir] = WALL;
    this->nb[dir]->nbwalls[(dir+2)%4] = WALL;
    this->nb[dir]->nb[(dir+2)%4] = NULL;
    this->nb[dir] = NULL;
  }else{
    this->nb[dir]->nbwalls[(dir+2)%4] = OPEN;
    this->nbwalls[dir] = OPEN;
  }
}

void Node::visit(){
  this->visited = VISITED;
}

// Directions can be converted to ints implicitly
Direction intToDir(int dir){
  dir = (dir % 4 + 4) % 4;
  switch(dir){
    case 0:
    return NORTH;
    case 1:
    return WEST;
    case 2:
    return SOUTH;
    case 3:
    return EAST;
  }
  return NONE;
}