#pragma once

#include <vector>

#include <aquila/transform/Spectrogram.h>

#include <fft_data.h>
#include <Amplitude.hpp>
#include <Frequency.hpp>
#include <WaveFile.h>
#include <unordered_map>

namespace utilities
{

	enum class FourierType : uint8_t
	{
		FastFourierTransformAquila,
		ShortTimeFourierTransformAquila,
		ShortTimeFourierTransform
	};

	const std::unordered_map<std::string, FourierType> stringToFourierType =
	{
		{"Fast Fourier Transform",FourierType::FastFourierTransformAquila},
		{"Short Time Fourier Transform",FourierType::ShortTimeFourierTransform},
		{"Aquila Short Time Fourier Transform",FourierType::ShortTimeFourierTransformAquila}
	};


	class FourierTransform
	{

	public:
		FourierTransform(const WaveFile& wav, unsigned int overlap = 0);

	private:
		std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>> calculate(const Aquila::Spectrogram& spectogram) const;
		std::pair < std::vector<double>, std::vector<fft_data::Amplitude>> calculateXY() const;
		Aquila::Spectrogram getSpectogram() const;
		

	public:

		//Aquilla method
		std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>> applyFastFourierTransformAquila() const;
		std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>> applyShortTimeFourierTransformFromAquila() const;

		//Our method
		std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>> applyShortTimeFourierTransform() const;
		
		unsigned int getOverlap();
		
		double getWindowDurationInSeconds();
		double getSamplesperFrame();
		double getSampleRate();
		const WaveFile& getWave();

	private:
		double m_sampleSize;
		int m_samplesPerFrame = 8192;
		unsigned int m_audioLength; // s
		double m_sampleRate;
		double m_timeIncrement;
		mutable unsigned int m_overlap;
		WaveFile m_wav;
	};
}