#pragma once

#include <string_view>
#include <vector>

#include <aquila/source/SignalSource.h>
#include <AudioFile.h>

class FileReader 
{
public:
	static std::vector<std::pair<Aquila::SignalSource, std::uint8_t>> loadDataset(std::string source);

public:
	static constexpr std::string_view kWavExtension = ".wav";
	static constexpr std::uint8_t kDesiredChannel = 0;

private:
	static AudioFile<double> loadAudioFile(std::string_view path);
	static Aquila::SignalSource convertToSignalSource(const AudioFile<double>& source);
};