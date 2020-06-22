#pragma once

#include <array>
#include <unordered_map>

#include <MidiFile.h>
#include <Options.h>

#include <Model.h>
#include <IExportFile.h>
#include <Frequency.hpp>
#include <Amplitude.hpp>
#include <Extractor.h>
#include <FourierTransform.h>
#include <Instrument.h>

class MidiExporter : public IExportFile
{
private:
	static constexpr int kFrequenciesCount = 128;

public:
	MidiExporter(const WaveFile& wav);

	~MidiExporter() = default;

public:
	void writeToFile(const std::string& fileName, utilities::FourierType type);
	std::map<int, int> writeToFile(const std::string& fileName, utilities::FourierType type, Extractor::ExtractionType extractionType, bool doLabels = false);
	std::map<int, int> writeToFile(const std::string& fileName, utilities::FourierType type, Extractor::ExtractionType extractionType, int extractNumber, bool doLabels = false);

	void openFileToWrite(const std::string& fileName) override;
	void endWrite() override;

private:
	std::map<int, int> startWritingToMidiFileLabelingWithModel(std::map<double, std::vector<int>>& notesAndWindows, std::vector<std::vector<double>>& mfccPerWindow, Model& model);
	std::map<int, int> writeMIDIWithLabels(std::map<fft_data::Frequency, std::vector<int>>& frequenciesAndAmplitudes);
	std::map<double, std::vector<int>> convertToMidiNotes(std::map<fft_data::Frequency, std::vector<int>>& frequencyAndAparitions);
	std::map<double, std::vector<int>> cutWindowDupicatesAndSortMidiNotes(std::map<double, std::vector<int>>& notesAndWindows);
	std::vector<std::vector<double>> calculateMfcc(Model& model);

	unsigned int solveOverlap();
	void startWritingToMidiFile(std::map<double, std::vector<int>>& notesAndWindows);
	void writeMIDI(std::map<fft_data::Frequency, std::vector<int>>& frequenciesAndAmplitudes);
	void defineOption(const std::string& option);
	void defineOptions(const std::vector<std::string>& options);
	void defineInstrument(Instruments::InstrumentType instr);
	void defineOutputType(char outputType = 'b');
	int calculateStartTick(double timeSpan) const;
	int calculateEndTick(double startTick, double duration) const;
	void addNoteToFile(int tarck, int startTick, int endTick, int channel, int key, double frequency);

private:
	constexpr std::array<double, kFrequenciesCount> computeFrequencies();
	constexpr double computeFrequency(int note);

	std::map<double, uint8_t> computeDifferences(double frequency);
	std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>> callSpecificFourierTransform(utilities::FourierType type);

private:
	smf::Options m_options;
	smf::MidiFile m_midiFile;

	int m_track;
	int m_channel;
	int m_instrument;
	int m_tpq;

private:
	utilities::FourierTransform m_fourierTransform;
};

