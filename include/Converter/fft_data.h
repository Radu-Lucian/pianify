#pragma once

#include<vector>

namespace fft_data
{
	//Freq and Ampl
	using Frame = std::pair<std::vector<double>, std::vector<double>>;
	using FFTData = std::vector<Frame>;

	struct Bounds
	{
		constexpr Bounds(double lowerBound, double upperBound) noexcept:
			lowerBound(lowerBound),
			upperBound(upperBound)
		{}
		
		double lowerBound;
		double upperBound;
	};
}
