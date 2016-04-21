#pragma once
#pragma once
#ifndef TR16_DiffAlg_h
#define TR16_DiffAlg_h

#include "Arduino.h"
#include "stdint.h"

class TR16_DiffAlg
{
public:
	TR16_DiffAlg();
	~TR16_DiffAlg();

	void calcSpeed(uint16_t x, uint16_t y);
	uint16_t getPowLeft();
	uint16_t getPowRight();

private:
	uint16_t inputPWM_X;
	uint16_t inputPWM_Y;
	
	float _xCoord;
	float _yCoord;

	float v;
	float w;

	float propL;
	float propR;

	uint16_t power[2];

	void calcVars();
	void calcProps();
	void calcPower();
};

#endif
