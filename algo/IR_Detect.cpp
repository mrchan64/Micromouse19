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
IRData window[REVIEW_WINDOW];
int counter = 0;
IRData baseline = {0.0f, 0.0f, 0.0f, 0.0f};
IRData total = {0.0f, 0.0f, 0.0f, 0.0f};

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

LinReg linear_regression(float* data, float ymean){
  float xmean = REVIEW_WINDOW/2;
  float numTotal = 0.0f;
  float x2Sum = 0.0f;
  float y2Sum = 0.0f;
  float xnorm, ynorm;
  for(int i = 0; i<REVIEW_WINDOW; i++){
    xnorm = i-xmean;
    ynorm = data[i]-ymean;
    numTotal += xnorm * ynorm;
    x2Sum += xnorm * xnorm;
    y2Sum += ynorm * ynorm;
  }
  float xstd = sqrt(x2sum / (REVIEW_WINDOW-1));
  float ystd = sqrt(y2sum / (REVIEW_WINDOW-1));
  LinReg lr;
  lr.r = numTotal / sqrt(x2Sum * y2Sum);
  lr.b = lr.r * ystd / xstd;
  lr.a = ymean - lr.b * xmean;
  return lr;
}

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
void normalize_sensors(){
  float data_lf[REVIEW_WINDOW];
  float data_l[REVIEW_WINDOW];
  float data_r[REVIEW_WINDOW];
  float data_rf[REVIEW_WINDOW];
  total.lf = 0.0f;
  total.l = 0.0f;
  total.r = 0.0f;
  total.rf = 0.0f;
  for(counter = 0; counter < REVIEW_WINDOW; counter++){
    window[counter] = read_all();
    data_lf[counter] = window[counter].lf;
    data_l[counter] = window[counter].l;
    data_r[counter] = window[counter].r;
    data_rf[counter] = window[counter].rf;
    total.lf += window[counter].lf;
    total.l += window[counter].l;
    total.r += window[counter].r;
    total.rf += window[counter].rf;
    IRData dat = window[counter];
  }
  baseline.lf = median(data_lf);
  baseline.l = median(data_l);
  baseline.r = median(data_r);
  baseline.rf = median(data_rf);
  // pc.printf("Base %f %f %f %f\n", baseline.lf, baseline.l, baseline.r, baseline.rf);
  counter = REVIEW_WINDOW - 1;
}

// Run one cycle;
void run_IR_cycle(){
  counter = (counter+1)%REVIEW_WINDOW;
  IRData old = window[counter];
  window[counter] = read_all();
  total.lf += window[counter].lf - old.lf;
  total.l += window[counter].l - old.l;
  total.r += window[counter].r - old.r;
  total.rf += window[counter].rf - old.rf;
}

void detect_wall(){

}

int read_side(){
  return 1;
}

IRData get_current_data(){
  return window[counter];
}

IRData get_curr_norm_data(){
  IRData norm_curr = {0.0f, 0.0f, 0.0f, 0.0f};
  norm_curr.lf = window[counter].lf-baseline.lf;
  norm_curr.l = window[counter].l-baseline.l;
  norm_curr.r = window[counter].r-baseline.r;
  norm_curr.rf = window[counter].rf-baseline.rf;
  return norm_curr;
}