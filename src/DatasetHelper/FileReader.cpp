#include "FileReader.h"

#include <filesystem>
#include <Instrument.h>

std::vector<std::pair<Aquila::SignalSource, std::uint8_t>> FileReader::loadDataset(std::string path)
{
	std::vector<std::pair<Aquila::SignalSource, std::uint8_t>> output;
	for (const auto& element: std::filesystem::directory_iterator(path))
	{
		if (element.is_directory())
		{
			std::cout <<"Reading: "<< element.path().filename() << std::endl;
			for (const auto& file : std::filesystem::directory_iterator(element))
			{
				if (file.path().extension() == kWavExtension)
				{
					auto loadedFile = loadAudioFile(file.path().string());
					auto signalSource = convertToSignalSource(loadedFile);
					auto name = element.path().filename().string();
					auto instrument = Instruments::kStringToInstrument.at(name);
					output.push_back({ signalSource, Instruments::kInstrumentToLabel.at(instrument) });
				}
			}
		}
	}
	return output;
}

AudioFile<double> FileReader::loadAudioFile(std::string_view path)
{
	AudioFile<double> file;
	file.load(path.data());
	return file;
}

Aquila::SignalSource FileReader::convertToSignalSource(const AudioFile<double>& source)
{
	return Aquila::SignalSource(source.samples[kDesiredChannel], source.getSampleRate());
}
