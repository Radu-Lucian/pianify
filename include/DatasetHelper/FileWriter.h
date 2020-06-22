#pragma once

#include <vector>
#include<fstream>

#include <aquila/source/SignalSource.h>

class FileWriter 
{
public:
	static constexpr int windowSize = 512;
	static constexpr int featureMFCC = 40;

public:
	static void write(const std::vector<std::pair<Aquila::SignalSource, uint8_t>>& files);

public:
	static std::vector<std::vector<double>> computeMfcc(const Aquila::SignalSource& source);
};

 
   