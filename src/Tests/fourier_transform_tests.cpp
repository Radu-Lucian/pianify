#include <catch.hpp>

#include <FourierTransform.h>
#include <WaveFile.h>
#include <directories.h>

TEST_CASE("Fourier")
{
	const auto audioTestSamplesDir = std::string(directories::getAudioTestSamplesDir());
	auto firstWav = WaveFile(audioTestSamplesDir + "/violin_Gs6_025_piano_arco-normal.wav");
	auto secondWav = WaveFile(audioTestSamplesDir + "/violin_Gs6_025_piano_pizz-normal.wav");
	auto thirdWav = WaveFile(audioTestSamplesDir + "/violin_Gs6_phrase_pianissimo_arco-normal.wav");
	auto fourthWav = WaveFile(audioTestSamplesDir + "/violin_Gs7_1_mezzo-forte_natural-harmonic.wav");

	SECTION("In-house STFT")
	{
		SECTION("First wav")
		{
			auto fft = utilities::FourierTransform(firstWav, 1024);
			auto x = fft.applyFastFourierTransformAquila();
		}
	}
}