#include "MidiExporter.h"

#include <InstrumentMappingTools.h>
#include <set>
#include "aquila/source/FramesCollection.h"
#include "aquila/transform/Mfcc.h"
#include "tiny_dnn/network.h"
#include <Model.h>


MidiExporter::MidiExporter(const WaveFile& wav)
	: m_track{ 0 },
	m_channel{ 0 },
	m_instrument{ 0 },
	m_fourierTransform{ wav , 1024 }
{
	m_midiFile = smf::MidiFile();
	m_options = smf::Options();

	m_midiFile.addTimbre(m_track, 0, m_channel, m_instrument);
	m_tpq = m_midiFile.getTPQ();
}

std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>> MidiExporter::callSpecificFourierTransform(utilities::FourierType type)
{
	switch (type)
	{
	case utilities::FourierType::FastFourierTransformAquila:
	{
		return m_fourierTransform.applyFastFourierTransformAquila();
	}
	case utilities::FourierType::ShortTimeFourierTransformAquila:
	{
		return m_fourierTransform.applyShortTimeFourierTransformFromAquila();
	}
	case utilities::FourierType::ShortTimeFourierTransform:
	{
		return m_fourierTransform.applyShortTimeFourierTransform();
	}
	default:
		return std::vector<std::pair<std::vector<fft_data::Frequency>, std::vector<fft_data::Amplitude>>>();
	}
}

void MidiExporter::writeToFile(const std::string& fileName, utilities::FourierType type)
{
	defineOption("o|output-file=s:" + fileName);

	auto vectorTransform = callSpecificFourierTransform(type);

	auto frequenciesAndAmplitudes = Extractor::getAmplitudesAndFrequenciesOnVariablek(vectorTransform, fft_data::Bounds(40, 7000));
	auto frequencyAndAparitions = Extractor::getUniqueFrequenciesAndWindowAparitions(frequenciesAndAmplitudes);

	auto midiFreqs = computeFrequencies();
	const double windowDurationInSeconds = m_fourierTransform.getWindowDurationInSeconds();
	constexpr double quarterDurationInSeconds = 0.5;
	const unsigned int overlapInTicks = ((m_fourierTransform.getSamplesperFrame() - m_fourierTransform.getOverlap()) /
		m_fourierTransform.getSampleRate()) * 240;
	const double tickDurationInSeconds = quarterDurationInSeconds / m_tpq;

	std::map<double, std::vector<int>> notesAndWindows;
	for (auto&[frequency, data] : frequencyAndAparitions)
	{
		for (auto indexCopy = 0; indexCopy < data.size(); indexCopy++)
		{
			std::vector<int> copyDataVec;
			auto midiNote = computeDifferences(frequency.value).begin()->second;
			auto copyData = notesAndWindows.find(midiNote);
			if (copyData != notesAndWindows.end())
			{
				copyDataVec = copyData->second;
				copyDataVec.push_back(data[indexCopy]);
				notesAndWindows[midiNote] = copyDataVec;
			}
			else
			{
				notesAndWindows[midiNote] = data;
			}
		}
	}
	for (auto&[note, data] : notesAndWindows)
	{
		std::sort(data.begin(), data.end());
		std::set<int> other;
		std::vector<int> finalVec;
		for (size_t i = 0; i < data.size() - 1; i++)
		{
			other.insert(data[i]);
		}
		for (auto& elem : other)
		{
			finalVec.push_back(elem);
		}
		notesAndWindows[note] = finalVec;
	}
	for (auto&[frequency, data] : notesAndWindows)
	{
		for (auto index = 0; index < data.size(); )
		{
			auto frequencyCounter = 1;
			for (auto index2 = index; index2 < data.size() - 1; ++index2)
			{
				if (data[index2 + 1] - data[index2] != 1) break;
				++frequencyCounter;
			}

			if (frequency != 0.0 && frequencyCounter != 1)
			{
				unsigned int startTick, endTick;
				auto windowNumber = data[index];
				auto correctionFactorStart = overlapInTicks * (windowNumber - 1);
				auto correctionFactorEnd = overlapInTicks * (windowNumber + frequencyCounter - 1);

				startTick = windowNumber * (windowDurationInSeconds / tickDurationInSeconds);
				if (windowNumber != 0)
				{
					startTick -= correctionFactorStart;
				}
				endTick = ((windowNumber + frequencyCounter) * (windowDurationInSeconds / tickDurationInSeconds)) - correctionFactorEnd;
				auto key = frequency;
				auto approximateInstrument = InstrumentMappingTools::getClosestInstrument(fft_data::Frequency(midiFreqs[key]));
				auto preferedAmplitude = InstrumentMappingTools::kPreferedAmplitudes.at(approximateInstrument);

				m_midiFile.addNoteOn(m_track, static_cast<int>(startTick), m_channel, key, preferedAmplitude.value);
				m_midiFile.addNoteOff(m_track, static_cast<int>(endTick), m_channel, key, preferedAmplitude.value);
			}
			index += frequencyCounter;
		}
	}
	m_track++;
	m_midiFile.sortTracks();
}

std::map<int, int> MidiExporter::writeToFile(const std::string & fileName, utilities::FourierType type, Extractor::ExtractionType extractionType, bool doLabels)
{
	std::map<int, int> totalAparitions;
	defineOption("o|output-file=s:" + fileName);

	auto vectorTransform = callSpecificFourierTransform(type);
	auto frequenciesAndAmplitudes = Extractor::getAmplitudesAndFrequenciesOnVariablek(vectorTransform, fft_data::Bounds(40, 7000));
	auto frequencyAndAparitions = Extractor::getUniqueFrequenciesAndWindowAparitions(frequenciesAndAmplitudes);

	if (!doLabels)
		writeMIDI(frequencyAndAparitions);
	else
		totalAparitions = writeMIDIWithLabels(frequencyAndAparitions);

	return totalAparitions;
}

std::map<int, int> MidiExporter::writeToFile(const std::string & fileName, utilities::FourierType type, Extractor::ExtractionType extractionType, int extractNumber, bool doLabels)
{
	std::map<int, int> totalAparitions;
	defineOption("o|output-file=s:" + fileName);

	auto vectorTransform = callSpecificFourierTransform(type);
	auto frequenciesAndAmplitudes = Extractor::getAmplitudesAndFrequenciesWIthPreDefinedK(vectorTransform, fft_data::Bounds(40, 7000), extractNumber);
	auto frequencyAndAparitions = Extractor::getUniqueFrequenciesAndWindowAparitions(frequenciesAndAmplitudes);

	if (!doLabels)
		writeMIDI(frequencyAndAparitions);
	else
		totalAparitions = writeMIDIWithLabels(frequencyAndAparitions);

	return totalAparitions;
}

std::map<int, int> MidiExporter::writeMIDIWithLabels(std::map<fft_data::Frequency, std::vector<int>>& frequencyAndAparitions)
{
	Model model;

	m_midiFile.addTrack();
	auto mfccPerWindow = calculateMfcc(model);
	auto midiNotes = convertToMidiNotes(frequencyAndAparitions);
	auto notesAndWindows = cutWindowDupicatesAndSortMidiNotes(midiNotes);
	auto totalLabels = startWritingToMidiFileLabelingWithModel(notesAndWindows, mfccPerWindow, model);
	
	m_track++;
	m_midiFile.sortTracks();
	return totalLabels;
}

std::map<int, int> MidiExporter::startWritingToMidiFileLabelingWithModel(std::map<double, std::vector<int>>& notesAndWindows, std::vector<std::vector<double>>& mfccPerWindow, Model& model)
{
	std::map<int, int> totalLabels;
	constexpr double quarterDurationInSeconds = 0.5;
	const double windowDurationInSeconds = m_fourierTransform.getWindowDurationInSeconds();
	const double tickDurationInSeconds = quarterDurationInSeconds / m_tpq;
	unsigned int overlapInTicks = solveOverlap();
	auto midiFreqs = computeFrequencies();

	for (auto&[frequency, data] : notesAndWindows)
	{
		for (auto index = 0; index < data.size(); )
		{
			std::map<int, int> labelsAndAparitions;
			auto frequencyCounter = 1;
			for (auto index2 = index; index2 < data.size() - 1; ++index2)
			{
				if (data[index2 + 1] - data[index2] != 1) break;
				++frequencyCounter;
			}

			if (frequency != 0.0 && frequencyCounter != 1)
			{
				unsigned int startTick, endTick;
				auto windowNumber = data[index];
				auto correctionFactorStart = overlapInTicks * (windowNumber - 1);
				auto correctionFactorEnd = overlapInTicks * (windowNumber + frequencyCounter - 1);

				startTick = windowNumber * (windowDurationInSeconds / tickDurationInSeconds);
				if (windowNumber != 0)
				{
					startTick -= correctionFactorStart;
				}

				for (auto frameIndex = windowNumber; frameIndex < windowNumber + frequencyCounter; ++frameIndex)
				{
					if (mfccPerWindow[frameIndex].size() == 0)
						continue;

					tiny_dnn::vec_t auxVec(mfccPerWindow[frameIndex].begin(), mfccPerWindow[frameIndex].end());
					auto res = model.predictOutput(auxVec);
					labelsAndAparitions[res]++;
				}

				totalLabels[0] += labelsAndAparitions[0];
				totalLabels[1] += labelsAndAparitions[1];

				auto maxElem = std::max_element(labelsAndAparitions.begin(), labelsAndAparitions.end(),
					[](const auto& p1, const auto& p2) {
					return p1.second < p2.second;
				});

				int instrumentIndex = 0;
				m_track = 0;
				if (maxElem->first == 1)
				{
					instrumentIndex = 56;
					m_track = 1;
				}

				m_midiFile.addTimbre(m_track, startTick, m_channel, instrumentIndex);

				endTick = ((windowNumber + frequencyCounter) * (windowDurationInSeconds / tickDurationInSeconds)) - correctionFactorEnd;
				auto key = frequency;

				addNoteToFile(m_track, startTick, endTick, m_channel, key, midiFreqs[key]);
			}
			index += frequencyCounter;
		}
	}
	return totalLabels;
}

std::vector<std::vector<double>> MidiExporter::calculateMfcc(Model& model)
{
	constexpr unsigned int windowSizeMfcc = 512;
	const int featureMfcc = 40;
	const unsigned int windowMeanMfcc = 16;


	Aquila::FramesCollection frames(*m_fourierTransform.getWave().getWave(), m_fourierTransform.getSamplesperFrame(), m_fourierTransform.getOverlap());
	Aquila::Mfcc mfcc(windowSizeMfcc);
	std::vector<std::vector<double>> mfccForWindows;
	std::vector<double> mfccMeanForSong;
	std::vector<std::vector<double>> mfccPerWindow;

	for (auto& frame : frames)
	{
		auto mfccResult = mfcc.calculate(frame, featureMfcc);
		mfccForWindows.push_back(mfccResult);

		for (size_t i = 0; i < mfccForWindows.size(); i += windowMeanMfcc)
		{
			double mean = 0;
			double sum = 0;
			for (size_t j = 0; j < mfccForWindows[0].size(); ++j)
			{
				int limit = i + windowMeanMfcc;
				if (mfccForWindows.size() < limit)
				{
					limit = mfccForWindows.size();
				}

				for (size_t index = i; index < limit; index++)
				{
					sum += mfccForWindows[index][j];
				}
				mean = sum / windowMeanMfcc;
				mfccMeanForSong.push_back(mean);
			}
			mfccPerWindow.push_back(mfccMeanForSong);
			mfccMeanForSong.clear();
		}
	}
	return mfccPerWindow;
}

void MidiExporter::writeMIDI(std::map<fft_data::Frequency, std::vector<int>>& frequencyAndAparitions)
{
	auto midiNotes = convertToMidiNotes(frequencyAndAparitions);
	auto notesAndWindows = cutWindowDupicatesAndSortMidiNotes(midiNotes);
	startWritingToMidiFile(notesAndWindows);

	m_track++;
	m_midiFile.sortTracks(); 
}

unsigned int MidiExporter::solveOverlap()
{
	unsigned int overlapInTicks = 0;
	if (m_fourierTransform.getOverlap() != 0)
	{
		overlapInTicks = ((m_fourierTransform.getSamplesperFrame() - m_fourierTransform.getOverlap()) /
			m_fourierTransform.getSampleRate()) * 240;
	}
	return overlapInTicks;
}

void MidiExporter::startWritingToMidiFile(std::map<double, std::vector<int>>& notesAndWindows)
{
	constexpr double quarterDurationInSeconds = 0.5;
	const double tickDurationInSeconds = quarterDurationInSeconds / m_tpq;
	const double windowDurationInSeconds = m_fourierTransform.getWindowDurationInSeconds();
	unsigned int overlapInTicks = solveOverlap();
	auto midiFreqs = computeFrequencies();

	for (auto&[frequency, data] : notesAndWindows)
	{
		for (auto index = 0; index < data.size(); )
		{
			auto frequencyCounter = 1;
			for (auto index2 = index; index2 < data.size() - 1; ++index2)
			{
				if (data[index2 + 1] - data[index2] != 1) break;
				++frequencyCounter;
			}

			if (frequency != 0.0 && frequencyCounter != 1)
			{
				unsigned int startTick, endTick;
				auto windowNumber = data[index];
				auto key = frequency;
				auto correctionFactorStart = overlapInTicks * (windowNumber - 1);
				auto correctionFactorEnd = overlapInTicks * (windowNumber + frequencyCounter - 1);

				startTick = windowNumber * (windowDurationInSeconds / tickDurationInSeconds);
				windowNumber != 0 ? startTick -= correctionFactorStart : startTick;
				endTick = ((windowNumber + frequencyCounter) * (windowDurationInSeconds / tickDurationInSeconds)) - correctionFactorEnd;
				
				addNoteToFile(m_track, startTick, endTick, m_channel, key, midiFreqs[key]);
			}
			index += frequencyCounter;
		}
	}
}

void MidiExporter::addNoteToFile(int track, int startTick, int endTick, int channel, int key, double frequency)
{
	auto approximateInstrument = InstrumentMappingTools::getClosestInstrument(fft_data::Frequency(frequency));
	auto preferedAmplitude = InstrumentMappingTools::kPreferedAmplitudes.at(approximateInstrument);

	m_midiFile.addNoteOn(m_track, startTick, m_channel, key, preferedAmplitude.value);
	m_midiFile.addNoteOff(m_track, endTick, m_channel, key, preferedAmplitude.value);
}

std::map<double, std::vector<int>> MidiExporter::convertToMidiNotes(std::map<fft_data::Frequency, std::vector<int>>& frequencyAndAparitions)
{
	std::map<double, std::vector<int>> notesAndWindows;
	for (auto&[frequency, data] : frequencyAndAparitions)
	{
		for (auto indexCopy = 0; indexCopy < data.size(); indexCopy++)
		{
			std::vector<int> copyDataVec;
			auto midiNote = computeDifferences(frequency.value).begin()->second;
			auto copyData = notesAndWindows.find(midiNote);
			if (copyData != notesAndWindows.end())
			{
				copyDataVec = copyData->second;
				copyDataVec.push_back(data[indexCopy]);
				notesAndWindows[midiNote] = copyDataVec;
			}
			else
			{
				notesAndWindows[midiNote] = data;
			}
		}
	}
	return notesAndWindows;
}

std::map<double, std::vector<int>> MidiExporter::cutWindowDupicatesAndSortMidiNotes(std::map<double, std::vector<int>>& notesAndWindows)
{
	for (auto&[note, data] : notesAndWindows)
	{
		std::sort(data.begin(), data.end());
		std::set<int> other;
		std::vector<int> finalVec;
		for (size_t i = 0; i < data.size() - 1; i++)
		{
			other.insert(data[i]);
		}
		for (auto& elem : other)
		{
			finalVec.push_back(elem);
		}
		notesAndWindows[note] = finalVec;
	}

	return notesAndWindows;
}

void MidiExporter::endWrite()
{
	std::string filename = m_options.getString("output-file");
	if (filename.empty())
	{
		if (m_options.getBoolean("hex"))
			m_midiFile.writeHex(std::cout);
	}
	else
		m_midiFile.write(filename);
	std::cout << "File duration in sec " << m_midiFile.getFileDurationInSeconds() << '\n';
}

void MidiExporter::openFileToWrite(const std::string& fileName)
{
	m_options.setModified("output-file", fileName);
	m_midiFile.setFilename(fileName);
}

void MidiExporter::defineInstrument(Instruments::InstrumentType instr)
{
	m_instrument = static_cast<int>(instr);
	m_midiFile.addTimbre(m_track, 0, m_channel, m_instrument);
}

void MidiExporter::defineOutputType(char outputType)
{
	defineOption("x|hex=" + outputType);
}

int MidiExporter::calculateStartTick(double timeSpan) const
{
	return int(timeSpan / 4.0 * m_tpq);
}

int MidiExporter::calculateEndTick(double startTick, double duration) const
{
	return startTick + int(calculateStartTick(duration));
}

/**
	 * Define the export options for the midi file.
	 * Each option has to follow this format:
	 *		i|identifier-name=i        <- dose not define a value that is asociated with the name
	 *			or
	 *		i|identifier-name=i:100    <- defines a value that is asociated with the name
	 *
	 * Options are "o|" for output file
	 *             "i|" for int type
	 *	f, d, c, b are for basic types also(float, double, char, bool)
	 *	x is for undefined type
*/
void MidiExporter::defineOption(const std::string& option)
{
	m_options.define(option);
}

void MidiExporter::defineOptions(const std::vector<std::string>& optionsVector)
{
	for (auto& option : optionsVector)
	{
		m_options.define(option);
	}
}

constexpr std::array<double, MidiExporter::kFrequenciesCount> MidiExporter::computeFrequencies()
{
	std::array<double, MidiExporter::kFrequenciesCount> values{ 0 };
	for (auto index = 0; index < kFrequenciesCount; ++index)
	{
		values[index] = computeFrequency(index);
	}
	return values;
}

constexpr double MidiExporter::computeFrequency(int note)
{
	return (440.0 * gcem::pow(2, (note - 69) / 12.0));
}

std::map<double, uint8_t> MidiExporter::computeDifferences(double frequency)
{
	auto midisFreq = computeFrequencies();
	std::map<double, uint8_t> values;

	for (auto index = 0; index < kFrequenciesCount; ++index)
	{
		double difference = std::abs(midisFreq[index] - frequency);
		values[difference] = index;
	}
	return values;
}
