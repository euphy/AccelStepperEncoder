#include "AccelStepperEncoder.h"


AccelStepperEncoder::AccelStepperEncoder(uint8_t interface, 
	uint8_t pin1, 
	uint8_t pin2, 
	uint8_t pin3, 
	uint8_t pin4, 
	bool enable)
	:AccelStepper(interface, 
		pin1, 
		pin2, 
		pin3, 
		pin4, 
		enable)
{
	// do nothing actually
}

void AccelStepperEncoder::addEncoder(Encoder *enc)
{
	_enc = enc;
}
Encoder* AccelStepperEncoder::getEncoder()
{
	return _enc;
}
long AccelStepperEncoder::readEnc()
{
	return _enc->read();
}
void AccelStepperEncoder::writeEnc(long value)
{
	_enc->write(value);
}