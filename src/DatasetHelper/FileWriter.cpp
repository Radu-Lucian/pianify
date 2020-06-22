#include "FileWriter.h"
#include <iostream>

#include <aquila/transform/Mfcc.h>
#include <aquila/source/FramesCollection.h>

void FileWriter::write(const std::vector<std::pair<Aquila::SignalSource, uint8_t>>& files)
{
	std::ofstream outputFileStream;
	std::vector<std::vector<double>> mfccTransform;

	outputFileStream.open("mfccSongs.data", std::ios::out);
	int counter = 0;
	for (auto&[song, label] : files)
	{
		auto mfccMeanForSong = computeMfcc(song);
		auto labelForSong = static_cast<int>(label);
		for (auto& vector : mfccMeanForSong)
		{

			if (vector.empty())
				continue;

			if (std::all_of(vector.begin(), vector.end(), [](auto elem) {return elem == 0; }))
				continue;

			outputFileStream << labelForSong;
			for (auto& element : vector)
			{
				outputFileStream << "," << element;
			}
			outputFileStream << std::endl;
		}
		counter++;
		std::cout << "Song counter mffc done:" << counter << "out of " << files.size() << std::endl;
	}
}

std::vector<std::vector<double>> FileWriter::computeMfcc(const Aquila::SignalSource & source)
{
	Aquila::FramesCollection frames(source, windowSize);
	std::vector<std::vector<double>> mfccForWindows;
	std::vector<std::vector<double>> returnMfcc;
	std::vector<double> mfccMeanForSong;
	Aquila::Mfcc mfcc(windowSize);

	for (auto& frame : frames)
	{
		auto mfccResult = mfcc.calculate(frame, featureMFCC);
		mfccForWindows.push_back(mfccResult);
	}

	for (size_t i = 0; i < mfccForWindows.size(); i += 16)
	{
		double mean = 0;
		double sum = 0;
		for (size_t j = 0; j < mfccForWindows[0].size(); ++j)
		{
			if (mfccForWindows.size() < i + 16)
			{
				break;
			}

			for (size_t index = i; index < 16 + i; index++)
			{
				sum += mfccForWindows[index][j];
			}
			mean = sum / 16;
			mfccMeanForSong.push_back(mean);
		}
		returnMfcc.push_back(mfccMeanForSong);
		mfccMeanForSong.clear();
	}
	return returnMfcc;
}
