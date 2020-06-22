#include "FourierTransform.h"

#include <aquila/source/FramesCollection.h>
#include <aquila/transform/FftFactory.h>

utilities::FourierTransform::FourierTransform(const WaveFile & wav, unsigned int overlap)
	: m_timeIncrement(1 / m_sampleRate),
	m_sampleSize(wav.getTotalNumberOfSamples()),
	m_audioLength(wav.getDuration()),
	m_sampleRate(wav.getSampleRate()),
	m_wav(wav),
	m_overlap(overlap)
{}

std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>> utilities::FourierTransform::calculate(const Aquila::Spectrogram& spectogram) const
{
	std::vector<fft_data::Frequency> xFFT(m_samplesPerFrame);
	std::vector<fft_data::Amplitude> yFFT(m_samplesPerFrame);

	const double frequencyIncrement = m_sampleRate / m_samplesPerFrame;

	for (int i = 0; i < m_samplesPerFrame; i++)
	{
		xFFT[i] = fft_data::Frequency((double)i * frequencyIncrement);
	}

	std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>> vectorTransform;

	for (size_t x = 0; x < spectogram.getFrameCount(); ++x)
	{
		for (size_t y = 0; y < spectogram.getSpectrumSize(); ++y)
		{
			Aquila::ComplexType point = spectogram.getPoint(x, y);
			double fourierFormula = std::abs(point) / (m_samplesPerFrame / 2);
			yFFT[y] = fft_data::Amplitude(fourierFormula);
		}
		vectorTransform.push_back({ xFFT, yFFT });
	}
	return vectorTransform;
}

std::pair < std::vector<double>, std::vector<fft_data::Amplitude>> utilities::FourierTransform::calculateXY() const
{
	std::vector<double> xTimeStamps(m_sampleSize);
	std::vector<fft_data::Amplitude> yAmplitude(m_sampleSize);

	for (int i = 0; i < m_sampleSize; i++)
	{
		xTimeStamps[i] = i * m_timeIncrement;
		yAmplitude[i] = fft_data::Amplitude(m_wav.getSample(i));
	}

	return { xTimeStamps , yAmplitude };
}

Aquila::Spectrogram utilities::FourierTransform::getSpectogram() const
{
	auto framesCollection = Aquila::FramesCollection(*m_wav.getWave(), m_samplesPerFrame, m_overlap);
	return Aquila::Spectrogram(framesCollection);
}

std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>> utilities::FourierTransform::applyFastFourierTransformAquila() const
{
	m_overlap = 0;
	auto spectogram = getSpectogram();
	return calculate(spectogram);
}

std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>> utilities::FourierTransform::applyShortTimeFourierTransformFromAquila() const
{
	auto spectogram = getSpectogram();
	m_overlap = m_samplesPerFrame - m_overlap;
	return calculate(spectogram);
}

std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>> utilities::FourierTransform::applyShortTimeFourierTransform() const
{
	auto[xTimeStamps, yAmplitude] = calculateXY();
	std::vector<fft_data::Frequency> xFFT(m_samplesPerFrame);
	std::vector<fft_data::Amplitude> yFFT(m_samplesPerFrame);

	const double freqIncrement = m_sampleRate / m_samplesPerFrame;

	for (size_t i = 0; i < m_samplesPerFrame; ++i)
	{
		xFFT[i] = fft_data::Frequency((double)i * freqIncrement);
	}

	std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>> vectorTransform;

	bool wasLastStep = false;
	for (int i = 0; i < m_sampleSize && !wasLastStep; i += m_overlap)
	{
		auto fft = Aquila::FftFactory::getFft(m_samplesPerFrame);
		std::vector<double> yCopy;

		int limit = m_samplesPerFrame + i;
		if (limit > m_sampleSize)
		{
			int remainIndex = 0;
			limit = m_sampleSize - i;
			for (size_t fftIndex = m_sampleSize - limit; fftIndex < m_sampleSize; ++fftIndex)
			{
				yCopy.push_back(yAmplitude[fftIndex].value);
				remainIndex = fftIndex;
			}
			for (size_t remaningIndex = limit; remaningIndex < m_samplesPerFrame; ++remaningIndex)
			{
				yCopy.push_back(0.0);
			}
			wasLastStep = true;
		}
		else
		{
			for (size_t fftIndex = i; fftIndex < limit; ++fftIndex)
			{
				yCopy.push_back(yAmplitude[fftIndex].value);
			}
		}
		auto spectrum = fft->fft(yCopy.data());

		for (size_t spectrumIndex = 0; spectrumIndex < spectrum.size(); ++spectrumIndex)
		{
			auto point = spectrum[spectrumIndex];
			yFFT[spectrumIndex] = fft_data::Amplitude(std::abs(point) / (m_samplesPerFrame / 2));
		}
		vectorTransform.push_back({ xFFT, yFFT });
	}
	return vectorTransform;
}

unsigned int utilities::FourierTransform::getOverlap()
{
	return m_overlap;
}

double utilities::FourierTransform::getWindowDurationInSeconds()
{
	return m_samplesPerFrame / m_sampleRate;
}

double utilities::FourierTransform::getSamplesperFrame()
{
	return m_samplesPerFrame;
}

double utilities::FourierTransform::getSampleRate()
{
	return m_sampleRate;
}

const WaveFile & utilities::FourierTransform::getWave()
{
	return m_wav;
}
