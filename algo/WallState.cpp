/**************************************************************
 *                                                            *
 *   This FILE manages the detected walls of the Micromouse   *
 *                                                            *
 **************************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "WallState.hpp"
#include "../lib/MiscIO.hpp"
#include "../lib/Serial.hpp"

// Variables -------------------------------------------------
CellWalls currWalls = {true, true, false};
CellWalls wallStateChange = {false, false, false};
// WallSlopes currSlopes;
WallVals currWallVals;
bool can_check_wall = true;

// METHODS ---------------------------------------------------
CellWalls get_curr_walls(){
  CellWalls retWalls;
  retWalls.left = currWalls.left;
  retWalls.right = currWalls.right;
  retWalls.front = currWalls.front;
  return retWalls;
}

void run_wall_update_cycle(){

  // float left_side_dat[REVIEW_WINDOW];
  // float right_side_dat[REVIEW_WINDOW];
  // for(int i = 0; i < REVIEW_WINDOW; i++){
  //   left_side_dat[i] = normwindow[i].lf;
  //   right_side_dat[i] = normwindow[i].rf;
  // }

  // Get regression values
  // y = a + bx
  // LinReg left_vals = linear_regression(left_side_dat, norm_total.lf/(float) REVIEW_WINDOW);
  // LinReg right_vals = linear_regression(right_side_dat, norm_total.rf/(float) REVIEW_WINDOW);

  // float front_val_l = norm_total.l/(float) REVIEW_WINDOW;
  // float front_val_r = norm_total.r/(float) REVIEW_WINDOW;

  // currSlopes.left = left_vals;
  // currSlopes.right = right_vals;

  // CHECK IF WALLS CAN BE UPDATED IN THIS STATE PLEASE
  if(!can_check_wall) return;

  // Verify something using the linreg slopes? and r val
  IRData curr_dat = get_all_ave();

  CellWalls newWalls;

  // FRONT PROCESSING
  // newWalls.front = front_val_l < f_thresh.l || front_val_r < f_thresh.r;
  // newWalls.left = left_vals.a /*+ left_vals.b * REVIEW_WINDOW / 2*/ < f_thresh.lf;
  // newWalls.right = right_vals.a /*+ right_vals.b * REVIEW_WINDOW / 2*/ < f_thresh.rf;
  // // pc.printf("%.3f\n\r", get_curr_norm_IR_data().lf);
  newWalls.front = curr_dat.l < f_thresh.l || curr_dat.r < f_thresh.r;
  newWalls.left = curr_dat.lf < f_thresh.lf;
  newWalls.right = curr_dat.rf < f_thresh.rf;

  currWallVals.left = curr_dat.lf;
  currWallVals.right = curr_dat.rf;


  wallStateChange.front = currWalls.front != newWalls.front;
  wallStateChange.left = currWalls.left != newWalls.left;
  wallStateChange.right = currWalls.right != newWalls.right;

  // pc.printf("%f\n\r", norm_total.l);

  currWalls = newWalls;
}

void reset_walls(){
  currWalls.left =  true;
  currWalls.right = true;
  currWalls.front = false;
  wallStateChange.left = false;
  wallStateChange.right = false;
  wallStateChange.front = false;
}