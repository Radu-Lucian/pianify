#include "Pianify.h"

#include <MidiExporter.h>

void Pianify::wavToMidi(std::string_view inputFilePath, std::string_view outputFilePath, 
	int extractNumber, utilities::FourierType fourierType)
{
	static std::unique_ptr<WaveFile> waveFile;
	static std::unique_ptr<MidiExporter> midiExporter;

	waveFile = std::make_unique<WaveFile>(inputFilePath);
	midiExporter = std::make_unique<MidiExporter>(*waveFile);

	midiExporter->writeToFile(outputFilePath.data(), fourierType, Extractor::ExtractionType::Fixed, extractNumber);
	midiExporter->endWrite();
}

std::unordered_map<Instruments::InstrumentType, double> Pianify::recognizeInstruments(std::string_view inputFilePath, std::string_view outputFilePath, int extractNumber)
{
	static std::unique_ptr<WaveFile> waveFile;
	static std::unique_ptr<MidiExporter> midiExporter;

	waveFile = std::make_unique<WaveFile>(inputFilePath);
	midiExporter = std::make_unique<MidiExporter>(*waveFile);

	static constexpr auto fourierType = utilities::FourierType::ShortTimeFourierTransformAquila;

	auto instrumentsAparitions = midiExporter->writeToFile(outputFilePath.data(), fourierType, Extractor::ExtractionType::Fixed, extractNumber, true);
	auto totalNumberOfWindows = std::accumulate(std::cbegin(instrumentsAparitions), std::cend(instrumentsAparitions), 0.0, [](int previous, const auto& pair) {return previous + pair.second; });

	auto result = std::unordered_map<Instruments::InstrumentType, double>();
	for (const auto&[label, aparitions] : instrumentsAparitions)
	{
		result[kLabelToInstrument.at(label)] = aparitions / totalNumberOfWindows;
	}
	midiExporter->endWrite();

	return result;
}
