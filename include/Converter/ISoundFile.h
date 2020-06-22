#include <vector>
#include <string>
#include <unordered_map>

class ISoundFile
{
public :
	enum class DurationType : uint8_t
	{
		Miliseconds,
		Seconds,
		Minutes
	};

public:
	virtual ~ISoundFile() = default;
	/**
	* Returns the duration of the file
	*
	* @param type ........
	* @return file duration
	*/
	virtual unsigned int getDuration(DurationType type = DurationType::Miliseconds) const = 0;

	/**
	 * Returns the data
	 * @return the vector containing all the samples values in the audio file
	 */
	virtual std::vector<double> getData()const = 0;

	/**
	* Returns the name of the file
	*
	* @return fileName
	*/
	virtual std::string getName() const = 0;

	/**
	* Returns the sample rate
	*
	* @return sample rate
	*/
	virtual double getSampleRate() const = 0;

	/**
	* Returns the time incremenet
	*
	* @return time increment
	*/
	virtual double getTimeIncrement() const = 0;

	/**
	* Returns total number of samples
	*
	* @return total number of samples
	*/
	virtual unsigned int getTotalNumberOfSamples() const = 0;

	/**
	* Returns the sample at the given index
	*
	* @return sample value
	*/
	virtual double getSample(int index) const = 0;
};
