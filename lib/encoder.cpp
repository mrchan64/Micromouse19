/**
 * This encoder class is dependent on using the "software" solution of using the mbed InterruptIn class
 * rather than using hardware interrupt pins.
 */

#include "encoder.hpp"


// Constructor
Encoder::Encoder(PinName channelA, PinName channelB)
	: myEncoder(channelA, channelB, NC, 1024, QEI::X4_ENCODING)
	{ }


int Encoder::read()
{
	return -myEncoder.getPulses();
}


void Encoder::reset()
{
	myEncoder.reset();
}


/*
// HARDWARE IMPLEMENTATION
// Constructor 
Encoder::Encoder()
{
	EncoderInitTIM2();
	EncoderInitTIM5();
}

// Constructor
Encoder_SW::Encoder_SW(PinName channelA_pin, PinName channelB_pin)
	: channelA(channelA_pin), channelB(channelB_pin)
{
	this->count = 0;
}


int Encoder_SW::read()
{
	return this->count;
}


void Encoder_SW::reset()
{
	this->count = 0;
}
*/

/**
 * "Hardware" encoder class implementation, based on 32-bit timers TIM2 & TIM5.
 */
