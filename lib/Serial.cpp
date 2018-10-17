/***********************************************************
 *                                                         *
 *   This FILE sets up the Serial used in the Micromouse   *
 *                                                         *
 ***********************************************************/

// AUTHOR: Matthew Chan
// CPP FILE

#include "Serial.hpp"

Serial pc(M_SERIAL_TX, M_SERIAL_RX, BAUD);

// METHODS ---------------------------------------------------
void print(char* str){
  pc.printf("%s", str);
}

void println(char* str){
  pc.printf("%s\n", str);
}