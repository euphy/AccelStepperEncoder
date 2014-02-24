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
	Serial.begin(57600);
	_motorToEncRatio = 1.51;
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

void AccelStepperEncoder::moveTo(long absolute)
{
	AccelStepper::moveTo(absolute);
	_targetEncPos = calculateEncoderTarget(_targetPos);
}
float AccelStepperEncoder::calculateEncoderTarget(long motorTarget)
{
	return motorTarget * _motorToEncRatio;
}

// Implements steps according to the current step interval
// You must call this at least once per step
// returns true if a step occurred
boolean AccelStepperEncoder::runSpeed()
{
    // Dont do anything unless we actually have a step interval
    if (!_stepInterval)
	return false;

    unsigned long time = micros();
    // Gymnastics to detect wrapping of either the nextStepTime and/or the current time
    unsigned long nextStepTime = _lastStepTime + _stepInterval;
    if (   ((nextStepTime >= _lastStepTime) && ((time >= nextStepTime) || (time < _lastStepTime)))
	|| ((nextStepTime < _lastStepTime) && ((time >= nextStepTime) && (time < _lastStepTime))))
    {
		// if the encoder witnessed the movement, then mark the step as taken
		if (_useEncoderForPositioning)
		{
			step(_currentPos);
			Serial.println("Step attempted.");
			// check the encoder
			float encPos = (float)readEnc();
			// work out 
			float encTarget = (float)_currentPos * _motorToEncRatio;
/*			
			Serial.print("After. _currentPos: ");
			Serial.print(_currentPos);
			Serial.print(", encPos: ");
			Serial.print(encPos);
			Serial.print(", encTarget: ");
			Serial.println(encTarget);
			
			Serial.print("high value: ");
			Serial.print(encTarget+_motorToEncRatio);
			Serial.print(", low value: ");
			Serial.println(encTarget-_motorToEncRatio);
			delay(1);
			*/
			delay(1);
			if ((encPos <= (encTarget+_motorToEncRatio)) && (encPos > (encTarget-_motorToEncRatio)))
			{
				Serial.println("Successful encoder Increment!");
				if (_direction == DIRECTION_CW)	_currentPos += 1;
				else _currentPos -= 1;
			}
			else
			{
				Serial.println("Unsuccessfully incremented, maybe a problem.");
				if ((_direction == DIRECTION_CW && encPos > encTarget))
				{ // we are AHEAD of where we should be going CW
					_currentPos += 1;
				}
				else if(_direction == DIRECTION_CCW && encPos < encTarget)
				{ // we are AHEAD of where we are going CCW
					_currentPos -= 1;
				}
				else
				{
				}
			}
		}
		else
		{
			step(_currentPos);
			Serial.println("Step attempted.");
			Serial.println("Increment!");
			if (_direction == DIRECTION_CW)
			{
				// Clockwise
				_currentPos += 1;
			}
			else
			{
				// Anticlockwise  
				_currentPos -= 1;
			}
			_lastStepTime = time;
		}
		return true;
    }
    else
    {
		return false;
    }
}

float AccelStepperEncoder::calculateMotorToEncRatio(int calibrationCycles, int calibrationSteps)
{
	_useEncoderForPositioning = false;
	writeEnc(0);
	long high = 0L;
	long low = 99999999L;
	delay(1000);

	// drive the motor back and forth a couple of times and see what the encoder measures
	for (int i=0; i<(calibrationCycles*2); i++) 
	{
		if (currentPosition() != 0) 
		{
			runToNewPosition(0);
			if (readEnc() < low) low = readEnc();
		}
		else 
		{
			runToNewPosition(calibrationSteps);
			if (readEnc() > high) high = readEnc();
		}
		Serial.println(readEnc());
	}
	long totalEncoderSize = high+low;
	Serial.print("Encoder measured ");
	Serial.print(totalEncoderSize);
	Serial.print(" steps for ");
	Serial.print(calibrationSteps);
	Serial.println(" motor steps.");
	
	float motorStepsPerEncStep = float(calibrationSteps) / float(totalEncoderSize);
	disableOutputs();
	_motorToEncRatio = motorStepsPerEncStep;
	_useEncoderForPositioning = true;

	Serial.print("That means there are ");
	Serial.print(motorStepsPerEncStep);
	Serial.println(" motor steps for each encoder step.");
	
	return motorStepsPerEncStep;
}
