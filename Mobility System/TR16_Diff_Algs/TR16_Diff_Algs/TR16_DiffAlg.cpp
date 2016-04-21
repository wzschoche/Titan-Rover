#include "TR16_DiffAlg.h"
#include "Arduino.h"
#include <math.h>

const uint16_t MAX_FWD = 1750;
const uint16_t MAX_REV = 1250;

TR16_DiffAlg::TR16_DiffAlg() {}

TR16_DiffAlg::~TR16_DiffAlg() {}

uint16_t TR16_DiffAlg::getPowLeft() {
	return power[0];
}

uint16_t TR16_DiffAlg::getPowRight() {
	return power[1];
}

void TR16_DiffAlg::calcSpeed(uint16_t x, uint16_t y) {

	//Set power to 0 throttle.
	power[0] = 1500;
	power[1] = 1500;

	//Filter input to ESC safe values.
	inputPWM_X = constrain(x, 1000, 2000);
	inputPWM_Y = constrain(y, 1000, 2000);

	//Map variables to cartesian coordinate format.
	_xCoord = map(inputPWM_X, 1000, 2000, -100, 100);
	_yCoord = map(inputPWM_Y, 1000, 2000, -100, 100);

	//Deadzone adjustment
	if (fabs(_xCoord) < 10) _xCoord = 0;
	if (fabs(_yCoord) < 10) _yCoord = 0;

	//If both coords within deadzone, return with 0 throttle.
	if (_xCoord == 0 && _yCoord == 0) return;
	else {
		_xCoord *= -1;

		calcVars();
		calcProps();
		calcPower();
	}
}

void TR16_DiffAlg::calcVars() {
	v = (100 - fabs(_xCoord)) * (_yCoord / 100) + _yCoord;
	w = (100 - fabs(_yCoord)) * (_xCoord / 100) + _xCoord;
}

void TR16_DiffAlg::calcProps() {
	propL = (v - w) / 2;
	propR = (v + w) / 2;
}

void TR16_DiffAlg::calcPower() {
	if (propL < 0) {
		power[0] = 1500 - ((1500 - MAX_REV) * (fabs(propL) / 100));
	}
	else {
		power[0] = 1500 + ((MAX_FWD - 1500) * (propL / 100));
	}

	if (propR < 0) {
		power[1] = 1500 - ((1500 - MAX_REV) * (fabs(propR) / 100));
	}
	else {
		power[1] = 1500 + ((MAX_FWD - 1500) * (propR / 100));
	}
}
