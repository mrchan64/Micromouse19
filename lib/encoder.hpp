/**
 * This encoder class is dependent on using the "software" solution of using the mbed InterruptIn class
 * rather than using hardware interrupt pins.
 */


#ifndef ENCODER_H
#define ENCODER_H

#include "mbed.h"
#include "QEI.hpp"


class Encoder
{
public:	
	Encoder(PinName channelA, PinName channelB);
	int read();
	void reset();

private:
	QEI myEncoder;
};

#endif
