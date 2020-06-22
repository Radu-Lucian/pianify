#include <catch.hpp>

#include <Amplitude.hpp>

TEST_CASE("Amplitude")
{
	SECTION("Default constructor")
	{
		fft_data::Amplitude amplitude;
		REQUIRE(amplitude.value == Approx(0.0));
	}

	SECTION("Explicit constructor with parameter")
	{
		fft_data::Amplitude amplitude(5.52325);
		REQUIRE(amplitude.value == Approx(5.52325));
	}
}