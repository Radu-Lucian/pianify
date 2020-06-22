#pragma once
#include <stdint.h>
#include <string>
#include <MidiFile.h>
#include <Options.h>
#include <random>
#include <iostream>

using namespace smf;

class MidiGenerator
{
public:
	MidiGenerator(std::string_view fileNameFormat, uint8_t numberOfFilesToGenerate, uint8_t noteDuration, uint8_t instrumentNumber, uint8_t offsetOfNotes);
	MidiGenerator(std::string_view fileNameFormat, uint8_t numberOfFilesToGenerate, uint8_t noteDuration, uint8_t instrumentNumber);

public:
	void generateMidiFile();

private:
	std::string getNoteDuration(uint8_t noteDuration) const;

private:
	std::string fileName;
	uint8_t numberOfFilesToGenerate;
	uint8_t noteDuration;
	uint8_t note;
	uint8_t instrumentType;
	uint8_t offsetNote;

private:
	static constexpr uint8_t k_track = 0;
	static constexpr uint8_t k_channel = 0;
	static constexpr uint8_t k_startTime = 2;
	static constexpr uint8_t k_velocity = 90;

public:
	static constexpr uint8_t k_maxMidiFiles = 87;
};