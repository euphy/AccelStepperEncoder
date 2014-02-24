#ifndef AccelStepperEncoder_h
#define AccelStepperEncoder_h
#include <HardwareSerial.h>

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
		boolean oldRunSpeed();
		boolean runSpeed();
		void moveTo(long absolute);
		float calculateMotorToEncRatio(int calibrationCycles=2, int calibrationSteps=1600);
	protected:
		Encoder *_enc;
		long _targetEncPos;
		float _motorToEncRatio;
		boolean _useEncoderForPositioning = true;
		float calculateEncoderTarget(long target);
};

#endif