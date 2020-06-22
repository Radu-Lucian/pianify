#define CATCH_CONFIG_MAIN  
#include <catch.hpp>

#include <Extractor.h>
#include <WaveFile.h>
#include <FourierTransform.h>

TEST_CASE("Wave file constructor")
{
	/*CHECK_THROWS_AS(WaveFile("randomFile.wav"), std::invalid_argument);
	CHECK_THROWS_AS(WaveFile("randomF2dsile.wav"), std::invalid_argument);
	CHECK_THROWS_AS(WaveFile("randomFile.xsls"), std::invalid_argument);*/
}

TEST_CASE("Wave File methods")
{
//	WaveFile wavFile(directories::getAudioSamplesDir() + "/avicii.wav");
//	Aquila::WaveFile aquilaWavFile(directories::getAudioSamplesDir() + "/avicii.wav");
//
//	REQUIRE(wavFile.getTotalNumberOfSamples() == aquilaWavFile.getSamplesCount());
//	REQUIRE(wavFile.getDuration() == aquilaWavFile.getAudioLength() / 1000);
//
////	REQUIRE(wavFile.getTimeIncrement() == Approx(1 / aquilaWavFile.getSampleFrequency()));
//	REQUIRE(wavFile.getSampleRate() == aquilaWavFile.getSampleFrequency());
//	REQUIRE(wavFile.getName() == aquilaWavFile.getFilename());
}

TEST_CASE("Extractor tests for ub.wav")
{
	/*WaveFile wav(directories::getAudioSamplesDir() + "/ub.wav");

	auto fft = utilities::FourierTransform(wav);
	auto vectorTransform = fft.applyFourier();

	REQUIRE(vectorTransform.size() == 2376);
	REQUIRE(vectorTransform.front().first.size() == 4096);
	REQUIRE(vectorTransform.front().second.size() == 4096);

	auto frequenciesAndAmplitudes = Extractor::getAmplitudesAndFrequenciesWIthPreDefinedK(vectorTransform, fft_data::Bounds(40, 10000));
	REQUIRE(frequenciesAndAmplitudes.size() == 2376);
	REQUIRE((*frequenciesAndAmplitudes.begin()).second.size() == Extractor::kExtractNumber);

	auto uniqueFreq = Extractor::getUniqueFrequenciesAndWindowAparitions(frequenciesAndAmplitudes);
	REQUIRE(uniqueFreq.size() == 330);
	REQUIRE((*uniqueFreq.begin()).second.size() == 619);*/
}
