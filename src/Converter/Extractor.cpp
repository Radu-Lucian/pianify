#include "Extractor.h"

#include <array>

std::map<int, std::unordered_multimap<fft_data::Frequency, fft_data::Amplitude>> Extractor::getAmplitudesAndFrequenciesWIthPreDefinedK(const std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>>& vectorTransform, const fft_data::Bounds & frequencyBounds, int extractNumber)
{
	std::map<int, std::unordered_multimap<fft_data::Frequency, fft_data::Amplitude>> frequenciesAmplitudesAndWindows;
	int windowNumber{ 0 };

	for (const auto&[frequencies, amplitudes] : vectorTransform)
	{
		std::multimap<fft_data::Amplitude, fft_data::Frequency, std::greater<fft_data::Amplitude>> temporary;
		for (auto index = 0; index < frequencies.size(); ++index)
		{
			temporary.insert({ fft_data::Amplitude(amplitudes[index].value), fft_data::Frequency(frequencies[index].value) });
		}

		int counter = 0;

		for (const auto&[amplitude, frequency] : temporary)
		{
			if (frequency.value < frequencyBounds.lowerBound || frequency.value > frequencyBounds.upperBound)
				continue;

			if (amplitude.value < 20.0)
				break;

			frequenciesAmplitudesAndWindows[windowNumber].insert({ frequency , amplitude });
			++counter;
			if (counter == extractNumber)
				break;
		}
		++windowNumber;
	}
	return frequenciesAmplitudesAndWindows;
}

/**
 * Save each frequency, then verifiy if the harmonic(s) of that was found.
 * if that was found, stop the increment of k and save it in a vector coresponding to each window
 * save the k in a vector for each window
 * else increment k
 */
std::map<int, std::unordered_multimap<fft_data::Frequency, fft_data::Amplitude>> Extractor::getAmplitudesAndFrequenciesOnVariablek(const std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>>& vectorTransform, const fft_data::Bounds & frequencyBounds)
{
	std::map<int, std::unordered_multimap<fft_data::Frequency, fft_data::Amplitude>> frequenciesAmplitudesAndWindows;
	const uint8_t kMaximumMidiNotes = 128;

	auto midiFreq = [kMaximumMidiNotes]()
	{
		std::array<double, 128> values{ 0 };
		for (auto index = 0; index < kMaximumMidiNotes; ++index)
		{
			values[index] = (440.0 * gcem::pow(2, (index - 69) / 12.0));
		}
		return values;
	};

	auto diferences = [&](double frequency)
	{
		auto midisFreq = midiFreq();
		std::map<double, uint8_t> values;

		for (auto index = 0; index < kMaximumMidiNotes; ++index)
		{
			double difference = std::abs(midisFreq[index] - frequency);
			values[difference] = index;
		}
		return values;
	};

	auto midiFreqs = midiFreq();

	int windowNumber{ 0 };
	int slideNumber{ 0 };
	int kIndex{ 0 };

	int count = 0;
	for (const auto&[frequencies, amplitudes] : vectorTransform)
	{
		std::multimap<fft_data::Amplitude, fft_data::Frequency, std::greater<fft_data::Amplitude>> temporary;
		for (auto index = 0; index < frequencies.size(); ++index)
		{
			temporary.insert({ fft_data::Amplitude(amplitudes[index].value), fft_data::Frequency(frequencies[index].value) });
		}

		int counter = 0;

		std::vector<int> extractNumberWindow(vectorTransform.size());
		std::vector<int> slideFrameCount;
		std::map<int, double> analizedFreqs;
		int k = 0;

		for (auto&[amplitude, frequency] : temporary)
		{
			if (frequency.value < frequencyBounds.lowerBound || frequency.value > frequencyBounds.upperBound)
				continue;

			auto note = diferences(frequency.value);
			auto noteHarmonic = diferences(frequency.value * 2);
			auto noteHarmonicTwo = diferences(frequency.value * 3);
			auto noteHarmonicThree = diferences(frequency.value * 4);

			int integerFrequency = static_cast<int>(midiFreqs[note.begin()->second]);
			int harmonicOfFrequency = static_cast<int>(midiFreqs[noteHarmonic.begin()->second]);
			int harmonicOfFrequencyTwo = static_cast<int>(midiFreqs[noteHarmonicTwo.begin()->second]);
			int harmonicOfFrequencyThree = static_cast<int>(midiFreqs[noteHarmonicThree.begin()->second]);

			analizedFreqs[harmonicOfFrequency] = amplitude.value;
			analizedFreqs[harmonicOfFrequencyTwo] = amplitude.value;
			analizedFreqs[harmonicOfFrequencyThree] = amplitude.value;

			if (analizedFreqs.find(integerFrequency) != analizedFreqs.end() || amplitude.value < 20.0)
			{
				slideFrameCount.push_back(k);
				k = 1;
				break;
			}
			else
			{
				++k;
			}
		}

		int sum = 0;
		for (auto& elem : slideFrameCount)
		{
			sum += elem;
		}
		sum /= slideFrameCount.size();

		extractNumberWindow[kIndex] = sum;

		if (slideNumber == 8)
			kIndex++;

		for (const auto&[amplitude, frequency] : temporary)
		{
			if (frequency.value < frequencyBounds.lowerBound || frequency.value > frequencyBounds.upperBound)
				continue;

			if (amplitude.value < 20.0)
				break;

			frequenciesAmplitudesAndWindows[windowNumber].insert({ frequency , amplitude });
			++counter;
			if (counter == extractNumberWindow[kIndex])
				break;
		}
		++windowNumber;
	}
	return frequenciesAmplitudesAndWindows;
}

std::map<fft_data::Frequency, std::vector<int>> Extractor::getUniqueFrequenciesAndWindowAparitions(const std::map<int, std::unordered_multimap<fft_data::Frequency, fft_data::Amplitude>>& data)
{
	std::map<fft_data::Frequency, std::vector<int>> mapOfUniqueFreq;
	for (const auto&[windowNumber, unorderedMapEntry] : data)
	{
		for (const auto&[freq, amplAndAparitions] : unorderedMapEntry)
		{
			mapOfUniqueFreq[freq].push_back(windowNumber);
		}
	}
	return mapOfUniqueFreq;
}
