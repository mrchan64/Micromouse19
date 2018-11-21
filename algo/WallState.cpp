/**************************************************************
 *                                                            *
 *   This FILE manages the detected walls of the Micromouse   *
 *                                                            *
 **************************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "WallState.hpp"

// Variables -------------------------------------------------
CellWalls currWalls = {true, true, false};
CellWalls wallStateChange = {false, false, false};
WallSlopes currSlopes;
can_check_wall = true;

// METHODS ---------------------------------------------------
CellWalls get_curr_walls(){
  CellWalls retWalls;
  retWalls.left = currWalls.left;
  retWalls.right = currWalls.right;
  retWalls.front = currWalls.front;
}

void run_wall_update_cycle(){

  float left_side_dat[REVIEW_WINDOW];
  float right_side_dat[REVIEW_WINDOW];
  for(int i = 0; i < REVIEW_WINDOW; i++){
    left_side_dat[i] = window[i].lf;
    right_side_dat[i] = window[i].rf;
  }

  // Get regression values
  // y = a + bx
  LinReg left_vals = linear_regression(left_side_dat, norm_total.lf/(float) REVIEW_WINDOW);
  LinReg right_vals = linear_regression(right_side_dat, norm_total.rf/(float) REVIEW_WINDOW);

  float front_val_l = norm_total.l/(float) REVIEW_WINDOW;
  float front_val_r = norm_total.r/(float) REVIEW_WINDOW;

  currSlopes = {left_vals, right_vals};

  // CHECK IF WALLS CAN BE UPDATED IN THIS STATE PLEASE
  if(!can_check_wall) return;

  // Verify something using the linreg slopes? and r val

  CellWalls newWalls;

  // FRONT PROCESSING
  newWalls.front = front_val_l < f_thresh.l || front_val_r < f_thresh.r;
  newWalls.left = left_vals.a + left_vals.b * REVIEW_WINDOW / 2 < f_thresh.lf;
  newWalls.right = right_vals.a + right_vals.b * REVIEW_WINDOW / 2 < f_thresh.lf;

  wallStateChange.front = currWalls.front != newWalls.front;
  wallStateChange.left = currWalls.left != newWalls.left;
  wallStateChange.right = currWalls.right != newWalls.right;

  currWalls = newWalls;
}

void reset_walls(){
  currWalls = {true, true, false};
  wallStateChange = {false, false, false};
}