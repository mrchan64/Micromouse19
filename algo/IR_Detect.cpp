/************************************************************
 *                                                          *
 *   This FILE manages IR normalization in the Micromouse   *
 *                                                          *
 ************************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "mbed.h"
#include "IR_Detect.hpp"
#include "../lib/Serial.hpp"

// OBJECTS ---------------------------------------------------
int counter = 0;
IRData baseline = {0.0f, 0.0f, 0.0f, 0.0f};
IRData scaling = {1.0f, 1.0f, 1.0f, 1.0f}; // multiply
IRData f_thresh = {0.0f, 0.0f, 0.0f, 0.0f};
IRData total = {0.0f, 0.0f, 0.0f, 0.0f};
IRData norm_total = {0.0f, 0.0f, 0.0f, 0.0f};

// secondary for that left side shit only lr should be used here
IRData baseline2 = {0.0f, 0.0f, 0.0f, 0.0f};
IRData f_thresh2 = {0.0f, 0.0f, 0.0f, 0.0f};
IRData norm_total2 = {0.0f, 0.0f, 0.0f, 0.0f};

IRData window[REVIEW_WINDOW];
IRData normwindow[REVIEW_WINDOW];
IRData normwindow2[REVIEW_WINDOW];

// LinReg leftReg = {0.0f, 0.0f, 0.0f};
// LinReg rightReg = {0.0f, 0.0f, 0.0f}; // Unused???

// METHODS ---------------------------------------------------
// median
float median(float* data){
  float temp;
  for(int i = 0; i< REVIEW_WINDOW-1; i++){
    if(data[i] > data[i+1]){
      temp = data[i];
      data[i] = data[i+1];
      data[i+1] = data[i];
      for(int j = i; j>0; j--){
        if(data[j]<data[j-1]){
          temp = data[j];
          data[j] = data[j-1];
          data[j-1] = temp;
        }else break;
      }
    }
  }
  return REVIEW_WINDOW%2 == 0 ? (data[REVIEW_WINDOW/2-1]+data[REVIEW_WINDOW/2])/2 : data[(REVIEW_WINDOW-1)/2];
}

// y = a + bx
// LinReg linear_regression(float* data, float ymean){
//   // float xmean = REVIEW_WINDOW/2;
//   // float numTotal = 0.0f;
//   // float x2Sum = 0.0f;
//   // float y2Sum = 0.0f;
//   // float xnorm, ynorm;
//   // for(int i = 0; i<REVIEW_WINDOW; i++){
//   //   xnorm = i-xmean;
//   //   ynorm = data[i]-ymean;
//   //   numTotal += xnorm * ynorm;
//   //   x2Sum += xnorm * xnorm;
//   //   y2Sum += ynorm * ynorm;
//   // }
//   // float xstd = sqrt(x2Sum / (REVIEW_WINDOW-1));
//   // float ystd = sqrt(y2Sum / (REVIEW_WINDOW-1));
//   LinReg lr;
//   // lr.r = numTotal / sqrt(x2Sum * y2Sum);
//   // lr.b = lr.r * ystd / xstd;
//   lr.r = 0.0f;
//   lr.b = 0.0f;
//   lr.a = ymean/* - lr.b * xmean*/;
//   return lr;
// }

// mean
IRData mean(){
  IRData mean_calc;
  mean_calc.lf = total.lf / REVIEW_WINDOW;
  mean_calc.l = total.l / REVIEW_WINDOW;
  mean_calc.r = total.r / REVIEW_WINDOW;
  mean_calc.rf = total.rf / REVIEW_WINDOW;
  return mean_calc;
}

// Rewrite all sensor data and renormalize;
void normalize_IR_sensors(){
  float data_lf[REVIEW_WINDOW];
  float data_l[REVIEW_WINDOW];
  float data_r[REVIEW_WINDOW];
  float data_rf[REVIEW_WINDOW];
  total.lf = 0.0f;
  total.l = 0.0f;
  total.r = 0.0f;
  total.rf = 0.0f;
  for(counter = 0; counter < REVIEW_WINDOW; counter++){
    // 1. SYNCHRONOUS READING
    // window[counter] = read_all();
    // 2. ASYNC READING
    while(!cycle_IR_async()){
      wait_us(100);
    }
    window[counter] = read_IR_async();
    data_lf[counter] = window[counter].lf;
    data_l[counter] = window[counter].l;
    data_r[counter] = window[counter].r;
    data_rf[counter] = window[counter].rf;
    total.lf += window[counter].lf;
    total.l += window[counter].l;
    total.r += window[counter].r;
    total.rf += window[counter].rf;
  }
  baseline.lf = median(data_lf);
  baseline.l = median(data_l);
  baseline.r = median(data_r);
  baseline.rf = median(data_rf);
  // pc.printf("Base %f %f %f %f\n", baseline.lf, baseline.l, baseline.r, baseline.rf);
  counter = REVIEW_WINDOW - 1;
}

// Scale current sense to 1 based on current baseline
// IMPORTANT: NOTE that distance to wall is inversely proportional to voltage level
//    This means: wall detected = high
//                no wall detected = low
// So the process here is to move forward (closer to wall) meaning voltage will go up
// This means that the scaling needs to be negative in order to have high level cutoff
// The reverse is also possible it is just more confusing when implementing
void scale_IR_sensors(){
  float data_l[REVIEW_WINDOW];
  float data_r[REVIEW_WINDOW];
  total.l = 0.0f;
  total.r = 0.0f;
  for(counter = 0; counter < REVIEW_WINDOW; counter++){
    while(!cycle_IR_async()){
      wait_us(100);
    }
    window[counter] = read_IR_async();
    data_l[counter] = window[counter].l;
    data_r[counter] = window[counter].r;
    total.l += window[counter].l;
    total.r += window[counter].r;
  }
  IRData temp;
  temp.l = median(data_l) - baseline.l;
  temp.r = median(data_r) - baseline.r;

  // run calculation scaling on l and r only
  scaling.lf = -1.0f; scaling.rf = -1.0f;
  scaling.l = -1.0f/temp.l;
  scaling.r = -1.0f/temp.r;

  // pc.printf("Base %f %f %f %f\n", baseline.lf, baseline.l, baseline.r, baseline.rf);
  counter = REVIEW_WINDOW - 1;
}

void set_IR_threshold_forward(){
  float data_l[REVIEW_WINDOW];
  float data_r[REVIEW_WINDOW];
  total.l = 0.0f;
  total.r = 0.0f;
  for(counter = 0; counter < REVIEW_WINDOW; counter++){
    while(!cycle_IR_async()){
      wait_us(100);
    }
    window[counter] = read_IR_async();
    data_l[counter] = window[counter].l;
    data_r[counter] = window[counter].r;
    total.l += window[counter].l;
    total.r += window[counter].r;
  }
  IRData temp;
  temp.l = (median(data_l) - baseline.l)*scaling.l;
  temp.r = (median(data_r) - baseline.r)*scaling.r;

  // run calculation scaling on l and r only
  f_thresh.l = temp.l * THRESHOLD_MULTIPLIER;
  f_thresh.r = temp.r * THRESHOLD_MULTIPLIER;

  // pc.printf("Base %f %f %f %f\n", baseline.lf, baseline.l, baseline.r, baseline.rf);
  counter = REVIEW_WINDOW - 1;
}

void set_IR_threshold_left(){
  float data_lf[REVIEW_WINDOW];
  total.lf = 0.0f;
  for(counter = 0; counter < REVIEW_WINDOW; counter++){
    while(!cycle_IR_async()){
      wait_us(100);
    }
    window[counter] = read_IR_async();
    data_lf[counter] = window[counter].lf;
    total.lf += window[counter].lf;
  }
  IRData temp;
  temp.lf = (median(data_lf) - baseline.lf)*scaling.lf;

  // run calculation scaling on lf only
  f_thresh.lf = temp.lf * THRESHOLD_MULTIPLIER;

  // pc.printf("Base %f %f %f %f\n", baseline.lf, baseline.l, baseline.r, baseline.rf);
  counter = REVIEW_WINDOW - 1;
}

void set_IR_threshold_right(){
  float data_rf[REVIEW_WINDOW];
  total.rf = 0.0f;
  for(counter = 0; counter < REVIEW_WINDOW; counter++){
    while(!cycle_IR_async()){
      wait_us(100);
    }
    window[counter] = read_IR_async();
    data_rf[counter] = window[counter].rf;
    total.rf += window[counter].rf;
  }
  IRData temp;
  temp.rf = (median(data_rf) - baseline.rf)*scaling.rf;

  // run calculation scaling on rf only
  f_thresh.rf = temp.rf * THRESHOLD_MULTIPLIER;

  // pc.printf("Base %f %f %f %f\n", baseline.lf, baseline.l, baseline.r, baseline.rf);
  counter = REVIEW_WINDOW - 1;
}

// Run one cycle;
void run_IR_cycle(){
  if(cycle_IR_async()){
    counter = (counter+1)%REVIEW_WINDOW;
    IRData old = window[counter];
    window[counter] = read_IR_async();
    total.lf += window[counter].lf - old.lf;
    total.l += window[counter].l - old.l;
    total.r += window[counter].r - old.r;
    total.rf += window[counter].rf - old.rf;

    // for norm data
    old = normwindow[counter];
    // IRData old2 = normwindow2[counter];
    IRData ndat;
    // IRData ndat2;
    ndat.lf = (window[counter].lf - baseline.lf) * scaling.lf;
    ndat.l = (window[counter].l - baseline.l) * scaling.l;
    ndat.r = (window[counter].r - baseline.r) * scaling.r;
    ndat.rf = (window[counter].rf - baseline.rf) * scaling.rf;
    // ndat2.lf = (window[counter].lf - baseline2.lf) * scaling.lf;
    normwindow[counter] = ndat;
    // normwindow2[counter] = ndat2;
    norm_total.lf += normwindow[counter].lf - old.lf;
    norm_total.l += normwindow[counter].l - old.l;
    norm_total.r += normwindow[counter].r - old.r;
    norm_total.rf += normwindow[counter].rf - old.rf;
    // norm_total2.lf += normwindow2[counter].lf - old2.lf;

  }
}

IRData get_current_IR_data(){
  return window[counter];
}

IRData get_curr_norm_IR_data(){
  return normwindow[counter];
}

IRData get_all_ave(){
  IRData ird;
  ird.lf = norm_total.lf/REVIEW_WINDOW;
  ird.l = norm_total.l/REVIEW_WINDOW;
  ird.r = norm_total.r/REVIEW_WINDOW;
  ird.rf = norm_total.rf/REVIEW_WINDOW;
  return ird;
}

// float get_lf_ave_2(){
//   return norm_total2.lf/REVIEW_WINDOW;
// }

void print_threshold_vals(){
  // print scaling
  // print f_thresh
  // print baseline
  pc.printf("   lf\t\t| l\t\t| r\t\t| rf\n\r");
  pc.printf("S: %.4f\t| %.4f\t| %.4f\t| %.4f\t| \n\r", scaling.lf, scaling.l, scaling.r, scaling.rf);
  pc.printf("F: %.4f\t| %.4f\t| %.4f\t| %.4f\t| \n\r", f_thresh.lf, f_thresh.l, f_thresh.r, f_thresh.rf);
  pc.printf("B: %.4f\t| %.4f\t| %.4f\t| %.4f\t| \n\r", baseline.lf, baseline.l, baseline.r, baseline.rf);
}

void setIRConstantsManual(IRData s, IRData f, IRData b){
  // dont need to run normalize, or scale, or set threshold
  scaling = s;
  f_thresh = f;
  baseline = b;
}