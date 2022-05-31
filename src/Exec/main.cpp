#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "Vibrato.h"
#include "AudioFileIf.h"
#include "ErrorDef.h"

using std::string;
using std::vector;
using std::unique_ptr;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	static const int fileBlockSize = 1023;

	CAudioFileIf* audioFileIn  = nullptr;
	CAudioFileIf* audioFileOut = nullptr;
	CAudioFileIf::FileSpec_t fileSpec;

	float** inputAudioData  = nullptr;
	float** outputAudioData = nullptr;
	std::vector<unique_ptr<Vibrato>> vibrato;

	string inputFilePath{};
	string outputFilePath{};
	float widthInSec{};
	float freqInHz{};

	if (argc < 5){
		cout << "Incorrect number of arguments!" << endl;
		return -1;
	}

	inputFilePath  = argv[1];
	outputFilePath = argv[2];
	widthInSec	   = atof(argv[3]);
	freqInHz       = atof(argv[4]);

	// Open Audio Files
	CAudioFileIf::create(audioFileIn);
	audioFileIn->openFile(inputFilePath, CAudioFileIf::FileIoType_t::kFileRead);
	if (!audioFileIn->isOpen()) {
		std::cout << "Couldn't open input file..." << std::endl;
		CAudioFileIf::destroy(audioFileIn);
		return -1;
	}
	audioFileIn->getFileSpec(fileSpec);

	CAudioFileIf::create(audioFileOut);
	audioFileOut->openFile(outputFilePath, CAudioFileIf::FileIoType_t::kFileWrite, &fileSpec);
	if (!audioFileOut->isOpen()) {
		std::cout << "Couldn't open output file..." << std::endl;
		CAudioFileIf::destroy(audioFileOut);
		return -1;
	}

	// Create instance
	for (int c = 0; c < fileSpec.iNumChannels; c++) {
		vibrato.emplace_back(new Vibrato());
	}

	// Initialize vibrato
	for (int c = 0; c < vibrato.size(); c++) {
		if (vibrato[c]->init(fileSpec.fSampleRateInHz) != Error_t::kNoError ||
			vibrato[c]->setParam(Vibrato::Param_t::widthInSec, widthInSec) != Error_t::kNoError ||
			vibrato[c]->setParam(Vibrato::Param_t::freqInHz, freqInHz) != Error_t::kNoError) {
			std::cout << "Invalid parameters..." << std::endl;
			CAudioFileIf::destroy(audioFileIn);
			CAudioFileIf::destroy(audioFileOut);
			vibrato[c].reset();
			return -1;
		}
	}

	// Allocate memory
	inputAudioData  = new float* [fileSpec.iNumChannels]{};
	outputAudioData = new float* [fileSpec.iNumChannels]{};
	for (int c = 0; c < fileSpec.iNumChannels; c++){
		inputAudioData[c]  = new float[fileBlockSize]{};
		outputAudioData[c] = new float[fileBlockSize]{};
	}

	// Process
	long long iNumFrames = fileBlockSize;
	while (!audioFileIn->isEof()){
		audioFileIn->readData(inputAudioData, iNumFrames);
		for (int c = 0; c < vibrato.size(); c++) {
			vibrato[c]->process(inputAudioData[c], outputAudioData[c], iNumFrames);
		}
		audioFileOut->writeData(outputAudioData, iNumFrames);
	}

	// Clean-up
	for (int c = 0; c < fileSpec.iNumChannels; c++) {
		delete[] inputAudioData[c];
		delete[] outputAudioData[c];
	}
	delete[] inputAudioData;
	delete[] outputAudioData;

	CAudioFileIf::destroy(audioFileOut);
	CAudioFileIf::destroy(audioFileOut);
	vibrato.clear();

	return 0;
}