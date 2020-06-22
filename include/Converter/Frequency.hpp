#pragma once

#include <gcem.hpp>

namespace fft_data
{
	struct Frequency
	{
		using data_type = double;

		static constexpr double kDefaultFrequency = 0.0;

		constexpr Frequency() noexcept;
		explicit constexpr Frequency(double value) noexcept;

		double value;
	};

}

inline constexpr fft_data::Frequency::Frequency() noexcept
	: value(kDefaultFrequency)
{
}

inline constexpr fft_data::Frequency::Frequency(double value) noexcept
	: value(value)
{
}

namespace std
{
	template <>
	struct hash<fft_data::Frequency>
	{
		bool operator()(const fft_data::Frequency& amplitude) const noexcept
		{
			return std::hash<double>{}(amplitude.value);
		}
	};

	template <>
	struct greater<fft_data::Frequency>
	{
		constexpr bool operator()(const fft_data::Frequency& lhs, const fft_data::Frequency& rhs) const noexcept
		{
			return lhs.value > rhs.value;
		}
	};

	template <>
	struct equal_to<fft_data::Frequency>
	{
		constexpr bool operator()(const fft_data::Frequency& lhs, const fft_data::Frequency& rhs) const noexcept
		{
			constexpr double epsilon = std::numeric_limits<double>::epsilon();
			return gcem::abs(lhs.value - rhs.value) <= epsilon;
		}
	};

	template <>
	struct less<fft_data::Frequency>
	{
		constexpr bool operator()(const fft_data::Frequency& lhs, const fft_data::Frequency& rhs) const noexcept
		{
			return lhs.value < rhs.value;
		}
	};
}


