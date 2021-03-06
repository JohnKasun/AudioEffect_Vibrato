#pragma once

#include <vector>
#include <memory>

#include "ErrorDef.h"
#include "Util.h"
#include "Lfo.h"
#include "RingBuffer.h"

class Vibrato{

public:
	enum Param_t {
		widthInSec,
		freqInHz,

		numParams
	};

	Vibrato();
	~Vibrato();

	Error_t init(float sampleRate);
	Error_t reset();

	Error_t setParam(Param_t param, float value);
	float getParam(Param_t param) const;

	Error_t process(const float* inputBuffer, float* outputBuffer, int numFrames);
private:
	bool isInParamRange(Param_t param, float value) const;

	float mParamValues[numParams]{};
	float mParamRanges[numParams][2]{};

	bool mIsInitialized = false;
	float mSampleRate = 0.0f;
	std::unique_ptr<CRingBuffer<float>> mDelayLine;
	std::unique_ptr<Lfo> mLfo;
};
