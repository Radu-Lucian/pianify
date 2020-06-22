#pragma once

#include <memory>
#include <string_view>

#include <aquila/source/WaveFile.h>

#include <ISoundFile.h>

class WaveFile : public ISoundFile
{
public:
	WaveFile(std::string_view fileName);

	WaveFile(const WaveFile& wav);

	unsigned int getDuration(ISoundFile::DurationType type = ISoundFile::DurationType::Miliseconds) const override;

	double getSampleRate() const override;

	double getTimeIncrement() const override;

	unsigned int getTotalNumberOfSamples() const override;

	std::vector<double> getData() const override;

	std::string getName() const override;

	double getSample(int index) const override;

	const std::unique_ptr<Aquila::WaveFile>& getWave() const;
private:
	std::unique_ptr<Aquila::WaveFile> m_waveFile;

private:
	static constexpr unsigned int kFileNotFoundDuration = 1000;
	static constexpr unsigned int kFileNotFoundSampleCount = 65537;
	static constexpr double kDefaultSampleRate = 44100.0;
	static constexpr std::string_view kWaveFileExtension = ".wav";
};
