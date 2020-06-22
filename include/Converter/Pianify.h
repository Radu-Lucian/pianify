#pragma once

#include <string_view>
#include <unordered_map>

#include <Instrument.h>
#include <FourierTransform.h>

class Pianify
{
public:
	static void wavToMidi(std::string_view inputFilePath, std::string_view outputFilePath, int extractNumber, utilities::FourierType fourierType);
	static std::unordered_map<Instruments::InstrumentType, double> recognizeInstruments(std::string_view inputFilePath, std::string_view outputFilePath, int extractNumber);

private:
	static inline const std::unordered_map<int, Instruments::InstrumentType> kLabelToInstrument =
	{
		{0, Instruments::InstrumentType::AcousticGrandPiano},
		{1, Instruments::InstrumentType::Trumpet}
	};
};