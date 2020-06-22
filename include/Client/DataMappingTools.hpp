#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <type_traits>
#include <iterator>

class DataMapper
{
public:
	template <typename T>
	std::enable_if_t<std::is_floating_point_v<T> || std::is_integral_v<T>, std::string> convertToString(const std::vector<std::vector<T>>& data);

	template <typename T>
	std::enable_if_t<std::is_floating_point_v<T> || std::is_integral_v<T>, std::string> convertToString(const std::vector<T>& data);

	template <typename T>
	std::enable_if_t<std::is_floating_point_v<T> || std::is_integral_v<T>, std::vector<T>> convertFromString(std::string_view data);

private:
	static constexpr std::string_view kElementsDelimiter{ "," };
	static constexpr std::string_view kEmptyString{ "" };
	static constexpr char kSequenceDelimiter{ ';' };
private:
	std::ostringstream m_outputStringStream;
	std::istringstream m_inputStringStream;
};

template<typename T>
inline std::enable_if_t<std::is_floating_point_v<T> || std::is_integral_v<T>, std::string> DataMapper::convertToString(const std::vector<std::vector<T>>& data)
{
	static std::string result;
	result.clear();

	for (const auto& vector : data)
	{
		std::copy(std::cbegin(vector), std::cend(vector), std::ostream_iterator<T>(m_outputStringStream, kElementsDelimiter.data()));
	}
	result = m_outputStringStream.str();
	result.pop_back();
	return result;
}

template<typename T>
inline std::enable_if_t<std::is_floating_point_v<T> || std::is_integral_v<T>, std::string> DataMapper::convertToString(const std::vector<T>& data)
{
	static std::string result;
	result.clear();

	std::copy(std::begin(data), std::end(data), std::ostream_iterator<T>(m_outputStringStream, kElementsDelimiter.data()));
	result = m_outputStringStream.str();
	result.pop_back();
	m_outputStringStream.str(kEmptyString.data());

	return result;
}

template<typename T>
inline std::enable_if_t<std::is_floating_point_v<T> || std::is_integral_v<T>, std::vector<T>> DataMapper::convertFromString(std::string_view data)
{
	static std::vector<T> result;
	result.clear();
	m_inputStringStream.clear();
	m_inputStringStream.str(data.data());

	for (T value; m_inputStringStream >> value;)
	{
		result.emplace_back(value);
	}

	return result;
}
