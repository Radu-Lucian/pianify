#include <catch.hpp>

#include <Frequency.hpp>

TEST_CASE("Frequency")
{
	SECTION("Default constructor")
	{
		fft_data::Frequency amplitude;
		REQUIRE(amplitude.value == Approx(0.0));
	}

	SECTION("Explicit constructor with parameter")
	{
		fft_data::Frequency amplitude(4.52325);
		REQUIRE(amplitude.value == Approx(4.52325));
	}
}