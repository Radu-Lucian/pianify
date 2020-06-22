#pragma once

#include <limits>
#include <gcem.hpp>

namespace fft_data
{
	struct Amplitude
	{
		using data_type = double;
		static constexpr double kDefaultAmplitude = 0.0;

		constexpr Amplitude() noexcept;
		explicit constexpr Amplitude(double value) noexcept;

		double value;
	};
}

inline constexpr fft_data::Amplitude::Amplitude() noexcept
	:value(kDefaultAmplitude)
{
}

inline constexpr fft_data::Amplitude::Amplitude(double value) noexcept
	:value(value)
{
}

namespace std
{
	template <>
	struct hash<fft_data::Amplitude>
	{
		bool operator()(const fft_data::Amplitude& amplitude) const noexcept
		{
			return std::hash<double>{}(amplitude.value);
		}
	};

	template <>
	struct greater<fft_data::Amplitude>
	{
		constexpr bool operator()(const fft_data::Amplitude& lhs, const fft_data::Amplitude& rhs) const noexcept
		{
			return lhs.value > rhs.value;
		}
	};

	template <>
	struct equal_to<fft_data::Amplitude>
	{
		constexpr bool operator()(const fft_data::Amplitude& lhs, const fft_data::Amplitude& rhs) const noexcept
		{
			constexpr double epsilon = std::numeric_limits<double>::epsilon();
			return gcem::abs(lhs.value - rhs.value) <= epsilon;
		}
	};

	template <>
	struct less<fft_data::Amplitude>
	{
		constexpr bool operator()(const fft_data::Amplitude& lhs, const fft_data::Amplitude& rhs) const noexcept
		{
			return lhs.value < rhs.value;
		}
	};
}



