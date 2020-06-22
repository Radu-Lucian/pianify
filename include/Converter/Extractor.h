#pragma once

#include <vector>
#include <map>
#include <unordered_map>

#include <Frequency.hpp>
#include <Amplitude.hpp>
#include <fft_data.h>

class Extractor
{
public:
	enum class ExtractionType : std::uint8_t
	{
		Variable,
		Fixed
	};
public:
	static std::map<int, std::unordered_multimap<fft_data::Frequency, fft_data::Amplitude>> getAmplitudesAndFrequenciesWIthPreDefinedK(const std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>>& vectorTransform, const fft_data::Bounds& frequencyBounds, int extractNumber);
	static std::map<int, std::unordered_multimap<fft_data::Frequency, fft_data::Amplitude>> getAmplitudesAndFrequenciesOnVariablek(const std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>>& vectorTransform, const fft_data::Bounds& frequencyBounds);
	
public:
	static std::map<fft_data::Frequency, std::vector<int>> getUniqueFrequenciesAndWindowAparitions(const std::map<int, std::unordered_multimap<fft_data::Frequency, fft_data::Amplitude>>& data);
};