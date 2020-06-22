#pragma once
#include <string>
#include <MidiGenerator.h>
#include "Instrument.h"

namespace utilities
{
	/*
	*	Used to create from scratch a database of notes in .mid file fomrat of a specific instrument
	*	Can be converted to .wav and then used to create the .csv needed for AI training
	*/
	void generateDataSetInMIDFileFormat(std::string nameOfInstrument, uint8_t offset, Instruments::InstrumentType instrument)
	{
		std::string instrumentName = nameOfInstrument;
		auto offsetNote = offset;
		auto instrumentNumber = Instruments::kInstrumentToLabel.at(instrument);
		auto maxDurationOfNotes = 8;
		auto minDurationOfNotes = 1;

		for (auto duration = minDurationOfNotes; duration < maxDurationOfNotes; ++duration)
		{
			auto element = std::make_unique<MidiGenerator>(instrumentName, MidiGenerator::k_maxMidiFiles, duration, instrumentNumber, offsetNote);
			element->generateMidiFile();
		}
	}

	/*
	*	Used to create a .csv file with MFCC features of .mid files found in datasets folder
	*	Used for training our AI
	*/
	void makeDataSetFromWAV(std::string endPath)
	{
		try
		{
			auto dataset = FileReader::loadDataset(endPath);
			FileWriter::write(dataset);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what();
		}
	}

	/*
	*	The First Part Of Our App
	*	Generates midi a midi file using the whole .wav file
	*/
	void generateMidi(std::string IOFileName)
	{
		std::string inputFileName = "/" + IOFileName + ".wav";
		std::string outFileName = IOFileName + ".mid";

		std::unique_ptr<WaveFile> wav;
		wav = std::make_unique<WaveFile>(directories::getAudioSamplesDir() + inputFileName);

		MidiExporter exportMidi(*wav);
		exportMidi.writeToFile(outFileName, utilities::FourierType::ShortTimeFourierTransformAquila);
		exportMidi.endWrite();
	}
}