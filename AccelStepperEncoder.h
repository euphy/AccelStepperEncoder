#ifndef AccelStepperEncoder_h
#define AccelStepperEncoder_h

#include "../AccelStepper/AccelStepper.h"
#include "Encoder.h"


class AccelStepperEncoder : public AccelStepper 
{
	public:
		AccelStepperEncoder(uint8_t interface = AccelStepper::FULL4WIRE, 
		uint8_t pin1 = 2, 
		uint8_t pin2 = 3, 
		uint8_t pin3 = 4, 
		uint8_t pin4 = 5, 
		bool enable = true);

		void addEncoder(Encoder *enc);
		Encoder* getEncoder();
		long readEnc();
		void writeEnc(long value);
	protected:
		Encoder *_enc;
};

#endif