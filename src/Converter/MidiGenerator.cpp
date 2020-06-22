#include "MidiGenerator.h"

MidiGenerator::MidiGenerator(std::string_view fileNameFormat, uint8_t numberOfFilesToGenerate, uint8_t noteDuration, uint8_t instrumentNumber, uint8_t offsetOfNote)
	: MidiGenerator(fileNameFormat, numberOfFilesToGenerate, noteDuration, instrumentNumber)
{
	offsetNote = offsetOfNote;
}

MidiGenerator::MidiGenerator(std::string_view fileNameFormat, uint8_t numberOfFilesToGenerate, uint8_t noteDuration, uint8_t instrumentNumber)
	: fileName(fileNameFormat),
	numberOfFilesToGenerate(numberOfFilesToGenerate),
	noteDuration(noteDuration),
	instrumentType(instrumentNumber)
{
	numberOfFilesToGenerate > k_maxMidiFiles ? numberOfFilesToGenerate = k_maxMidiFiles : k_maxMidiFiles;
}

void MidiGenerator::generateMidiFile()
{
	int count = 0;

	while (count++ < numberOfFilesToGenerate)
	{
		MidiFile midifile;
		midifile.addTimbre(k_track, 0, k_channel, instrumentType);

		int tpq = midifile.getTPQ();
		int starttick = int(k_startTime / 4.0 * tpq);
		int endtick = starttick + int(noteDuration / 4.0 * tpq);

		note++;
		midifile.addNoteOn(k_track, starttick, k_channel, note - offsetNote, k_velocity);
		midifile.addNoteOff(k_track, endtick, k_channel, note - offsetNote, k_velocity);

		midifile.sortTracks();

		std::string filename = fileName + std::to_string(count) + getNoteDuration(noteDuration) + ".mid";
		midifile.write(filename);
	}
}

std::string MidiGenerator::getNoteDuration(uint8_t noteDuration) const
{
	switch (noteDuration)
	{
	case 1:
		return "_very_short";
	case 2:
		return "_short";
	case 3:
		return "_medium_short";
	case 4:
		return "_medium";
	case 5:
		return "_long_medium";
	case 6:
		return "_long";
	case 7:
		return "_very_long";
	default:
		return "";
	}
}
