#pragma once

#include <vector>
#include <string>

#include <FourierTransform.h>

class IExportFile
{
protected:
	using FrequenciesAndAmplitudes = std::vector<std::pair<double, double>>;

public:

	virtual ~IExportFile() = default;

	/**
	* Writes to the file format
	*
	* @param extracted info for export
	* @return void
	*/
	virtual void writeToFile(const std::string& fileName, utilities::FourierType type) = 0;

	/**
	* Ends write process
	*
	* @return void
	*/
	virtual void endWrite() = 0;

	/**
	* Opens an existing file or creates a new file to write on
	*
	* @param file name to open/create
	* @return void
	*/
	virtual void openFileToWrite(const std::string& fileName) = 0;
};