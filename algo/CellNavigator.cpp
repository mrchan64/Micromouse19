/**************************************************************
 *                                                            *
 *   This FILE manages the CellNavigation of the Micromouse   *
 *                                                            *
 **************************************************************/

// AUTHOR: Matthew Chan
// CPP_FILE

#include "CellNavigator.hpp"

// VARIABLES -------------------------------------------------
float degreeTracker = 0.0f;
float distOffset = 0.0f;  // add to dist
float cellStraightDist = 0.0f;
int left_mode_counter = 0;
int right_mode_counter = 0;
CellType currCellType = STRAIGHT;
MotorSpeed curr_motor_speed = {0.0f, 0.0f};
int abs_orient = 0;
bool cellFinished = false;

// METHODS ---------------------------------------------------
void reset_nav() {
  degreeTracker = 0.0f;
  distOffset = 9.0f;
  cellStraightDist = 0.0f;
  currCellType = STRAIGHT;
  curr_motor_speed = {0.0f, 0.0f};
  abs_orient = 0;
  left_mode_counter = 0;
  right_mode_counter = 0;
  cellFinished = false;
}

void new_cell() {
  degreeTracker = 0.0f;
  distOffset = 0.0f;
  cellStraightDist = 0.0f;
  left_mode_counter = 0;
  right_mode_counter = 0;
  cellFinished = false;
}

void run_cell_nav_cycle(){

  // STATE MACHINE
  switch(currCellType){
    case STRAIGHT:
    cell_straight();
    break;
    case CURVE_90:
    cell_curve_90();
    break;
    case U_TURN:
    cell_u_turn();
    break;
    case CURVE_45:
    cell_curve_45();
    break;
    case STRAIGHT_45:
    cell_straight_45();
    break;
  }
}

void move_straight(CellWalls currWalls){
  if(currWalls.left){
    float step_del = currSlopes.left.b * BTOSTEP + currSlopes.left.a * ATOSTEP;
    m_left.setStepAboveSpeed(curr_motor_speed.left, step_del);
  }

  if(currWalls.right){
    float step_del = currSlopes.right.b * BTOSTEP + currSlopes.right.a * ATOSTEP;
    m_right.setStepAboveSpeed(curr_motor_speed.right, step_del);
  }

}

void move_diagonal(){
  float enc_dist = m_left.getDist();
  float enc_delta = enc_dist - m_right.getDist(); // delta is positive if left is higher
  float enc_step = enc_delta / ENC_DEL_TO_STEP;
  int speed_step = abs(enc_delta) < 1 ? 0 : round(enc_step);
  m_left.setStepAboveSpeed(curr_motor_speed.left, step_del);
  m_right.setStepAboveSpeed(curr_motor_speed,right, -step_del);

}

/* GOING STRAIGHT THROUGH THE CELL
 * HOPEFULLY THIS DOESNT OVERWRITE WALLS ITS BEEN THROUGH
 */
void cell_straight(){
  CellWalls currWalls = get_curr_walls();
  move_straight(currWalls);
  float n_dist = m_left.getDist();
  float dist = n_dist - distOffset;

  int facing_dir = orientation_to_direction(abs_orient);
  int left_dir = (facing_dir + 1) % 4;
  int right_dir = (facing_dir + 3) % 4;
  int back_dir = (facing_dir + 2) % 4;

  Node currCell = currMaze[currLoc.x][currLoc.y];
  Node nextCell = currCell.nb[facing_dir];

  int left_wall_state = nextCell.nbwalls[left_dir];
  int right_wall_state = nextCell.nbwalls[right_dir];

  // check if there is wall in front
  if(currWalls.front){
    nextCell.resolveWall(facing_dir, true);
  }else{
    currCell.resolveWall(facing_dir, false);
  }

  // for left wheel
  switch(left_mode_counter) {
    case 0: // First section where wall existence is known
    if(dist > SCAN_WALL_ZONE) left_mode_counter = 1;
    break;

    case 1: // Second section where wall existence is questionable
    if((left_wall_state == 2) != currWalls.left){
      /* If wall state changes, we can verify where to change
       * cell bassed on position and populate next cell
       * IF NEW DETECTED WALL, it means it could go down again and be
       * a perpendicular wall */
      if(currWalls.left){
        left_mode_counter = 2;
        // SET WALL OFFSET
        distOffset = SCAN_WALL_START_ALIGN - dist;
      }else{
        // populate because no wall means it cant be perp wall
        nextCell.resolveWall(left_dir, true);
        left_mode_counter = 3;
        // SET WALL OFFSET
        distOffset = SCAN_WALL_END_ALIGN - dist;
      }
    }
    if(dist > SCAN_WALL_ZONE_END){
      left_mode_counter = 3;
      nextCell.resolveWall(left_dir, left_wall_state);
    }
    break;

    case 2: // Third section where we wait if wall disappears
    if(!currWalls.left){
      nextCell.nb[left_dir].resolveWall(back_dir, true);
      nextCell.resolveWall(left_dir, false);
      left_mode_counter = 3;
      // SET WALL OFFSET
      distOffset = SCAN_WALL_END_ALIGN - dist;
    }
    if(dist > SCAN_WALL_ZONE_END){
      left_mode_counter = 3;
      nextCell.resolveWall(left_dir, true);
    }
    break;

    case 3: // Fourth section where we do our thing until end of cell
    if(dist >= STRAIGHT_END){
      cellFinished = true;
    }
    break;
  }

  // for right wheel
  switch(right_mode_counter) {
    case 0: // First section where wall existence is known
    if(dist > SCAN_WALL_ZONE) right_mode_counter = 1;
    break;

    case 1: // Second section where wall existence is questionable
    if((right_wall_state == 2) != currWalls.right){
      /* If wall state changes, we can verify where to change
       * cell bassed on position and populate next cell
       * IF NEW DETECTED WALL, it means it could go down again and be
       * a perpendicular wall */
      if(currWalls.right){
        right_mode_counter = 2;
        // SET WALL OFFSET
        distOffset = SCAN_WALL_START_ALIGN - dist;
      }else{
        // populate because no wall means it cant be perp wall
        nextCell.resolveWall(right_dir, true);
        right_mode_counter = 3;
        // SET WALL OFFSET
        distOffset = SCAN_WALL_END_ALIGN - dist;
      }
    }
    if(dist > SCAN_WALL_ZONE_END){
      right_mode_counter = 3;
      nextCell.resolveWall(right_dir, right_wall_state);
    }
    break;

    case 2: // Third section where we wait if wall disappears
    if(!currWalls.right){
      nextCell.nb[right_dir].resolveWall(back_dir, true);
      nextCell.resolveWall(right_dir, false);
      right_mode_counter = 3;
      // SET WALL OFFSET
      distOffset = SCAN_WALL_END_ALIGN - dist;
    }
    if(dist > SCAN_WALL_ZONE_END){
      right_mode_counter = 3;
      nextCell.resolveWall(right_dir, true);
    }
    break;

    case 3: // Fourth section where we do our thing until end of cell
    if(dist >= STRAIGHT_END){
      cellFinished = true;
    }
    break;
  }

  // SET CELLSTRAIGHTDIST : THIS IS THE CALULATED DISTANCE ALONG THE CELL
  cellStraightDist = n_dist - distOffset;
}

void cell_curve_90(){
  CellWalls currWalls = get_curr_walls();
}

int orientation_to_direction(){
  switch(((abs_orient % 360) + 360) % 360){
    case 0:
    return 0;
    case 90:
    return 1;
    case 180:
    return 2;
    case 270:
    return 3;
    default:
    return -1;
  }
}