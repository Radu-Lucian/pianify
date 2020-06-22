#include "InstrumentMappingTools.h"

#include <gcem.hpp>
#include <limits>

const std::unordered_map<Instruments::InstrumentType, fft_data::Amplitude> InstrumentMappingTools::kPreferedAmplitudes =
{
	{Instruments::InstrumentType::Default,    fft_data::Amplitude(90)},
	{Instruments::InstrumentType::SteelDrums, fft_data::Amplitude(55)},
	{Instruments::InstrumentType::SynthDrum,  fft_data::Amplitude(55)}
};

const std::unordered_map<Instruments::InstrumentType, fft_data::Frequency> InstrumentMappingTools::kFundamentalFrequencies =
{
	{Instruments::InstrumentType::SteelDrums,			fft_data::Frequency(155)},
	{Instruments::InstrumentType::SynthDrum,			fft_data::Frequency(185)},
	{Instruments::InstrumentType::ReverseCymbal,		fft_data::Frequency(4000)},
	{Instruments::InstrumentType::ElectricGuitarClean,  fft_data::Frequency(739.50)},
	{Instruments::InstrumentType::ElectricBassFinger,	fft_data::Frequency(151.50)},
	{Instruments::InstrumentType::AcousticGuitarSteel,	fft_data::Frequency(739.50)}
};

const std::unordered_map<Instruments::InstrumentType, std::pair<fft_data::Frequency, fft_data::Frequency>> InstrumentMappingTools::kInstrumentsFrequenciesRanges =
{
	{Instruments::InstrumentType::SteelDrums,  {fft_data::Frequency(60),   fft_data::Frequency(250)}},
	{Instruments::InstrumentType::SynthDrum,   {fft_data::Frequency(120),  fft_data::Frequency(250)}},
};

Instruments::InstrumentType InstrumentMappingTools::getClosestInstrument(const fft_data::Frequency & frequency)
{
	std::vector<Instruments::InstrumentType> possibleInstruments;
	for (const auto[instrument, frequencyRange] : kInstrumentsFrequenciesRanges)
	{
		if (const auto[lowerBound, upperBound] = frequencyRange;
			frequency.value >= lowerBound.value && frequency.value <= upperBound.value)
		{
			possibleInstruments.push_back(instrument);
		}
	}
	return getClosestInstrument(frequency, possibleInstruments);
}

Instruments::InstrumentType InstrumentMappingTools::getClosestInstrument(const fft_data::Frequency & frequency, const std::vector<Instruments::InstrumentType>& possibleInstruments)
{
	double minimumDistance = std::numeric_limits<double>::max();
	Instruments::InstrumentType closestInstrument = Instruments::InstrumentType::Default;
	for (const auto instrument : possibleInstruments)
	{
		const auto fundamentalFrequency = kFundamentalFrequencies.at(instrument);
		const auto distance = gcem::abs(fundamentalFrequency.value - frequency.value);
		if (distance < minimumDistance)
		{
			minimumDistance = distance;
			closestInstrument = instrument;
		}
	}
	return closestInstrument;
}
