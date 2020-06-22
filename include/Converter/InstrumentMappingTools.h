#pragma once

#include <unordered_map>
#include <array>
#include <vector>

#include <Instrument.h>
#include <Frequency.hpp>
#include <Amplitude.hpp>

class InstrumentMappingTools
{
public:
	static const std::unordered_map<Instruments::InstrumentType, fft_data::Amplitude> kPreferedAmplitudes;
	static const std::unordered_map<Instruments::InstrumentType, std::pair<fft_data::Frequency, fft_data::Frequency>> kInstrumentsFrequenciesRanges;
	static const std::unordered_map<Instruments::InstrumentType, fft_data::Frequency> kFundamentalFrequencies;
public:
	static Instruments::InstrumentType getClosestInstrument(const fft_data::Frequency& frequency);
	static Instruments::InstrumentType getClosestInstrument(const fft_data::Frequency& frequency, const std::vector<Instruments::InstrumentType>& possibleInstruments);
};