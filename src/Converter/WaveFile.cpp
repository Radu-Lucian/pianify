#include "WaveFile.h"

#include <filesystem>

WaveFile::WaveFile(std::string_view fileName)
{
	if (const auto path = std::filesystem::path(fileName); path.extension() != kWaveFileExtension)
		throw std::invalid_argument("Invalid extension.");

	m_waveFile = std::make_unique<Aquila::WaveFile>(fileName.data());
	if (m_waveFile->getSamplesCount() == kFileNotFoundSampleCount && m_waveFile->getAudioLength() == kFileNotFoundDuration
				&& m_waveFile->getSampleFrequency() != kDefaultSampleRate)
		throw std::invalid_argument("Invalid file name.");
}

WaveFile::WaveFile(const WaveFile& wav)
{
	m_waveFile = std::make_unique<Aquila::WaveFile>(wav.getWave()->getFilename());
}

unsigned int WaveFile::getDuration(ISoundFile::DurationType type) const
{
	static constexpr int kToSeconds = 1000;
	return m_waveFile->getAudioLength() / kToSeconds;
}

double WaveFile::getSampleRate() const
{
	return m_waveFile->getSampleFrequency();
}

double WaveFile::getTimeIncrement() const
{
	return 1 / getSampleRate();
}

unsigned int WaveFile::getTotalNumberOfSamples() const
{
	return m_waveFile->getSamplesCount();
}

std::vector<double> WaveFile::getData() const
{
	std::vector<double> data(m_waveFile->getSamplesCount());

	for (int i = 0; i < data.size(); i++)
	{
		data[i] = (m_waveFile->sample(i));
	}

	return data;
}

std::string WaveFile::getName() const
{
	return m_waveFile->getFilename();
}

const std::unique_ptr<Aquila::WaveFile>& WaveFile::getWave () const
{
	return m_waveFile;
}

double WaveFile::getSample(int index) const
{
	return m_waveFile->sample(index);
}
