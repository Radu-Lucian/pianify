#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/read_until.hpp>

#include <string_view>
#include <iostream>

#include <DataMappingTools.hpp>


class Client
{
public:
	Client();

	void connect(std::string_view ipAddress, int port);
	
	template <typename T>
	void sendData(const std::vector<std::vector<T>>& data);

	template <typename T>
	void sendData(const std::vector<T>& data);

	template <typename T = int>
	std::vector<T> receiveData();

private:
	template <typename T>
	void sendDataGenerically(const T& data);

private:
	static constexpr std::string_view kReadLimit = "\n";

private:
	boost::asio::io_service m_ioService;
	boost::asio::ip::tcp::socket m_socket;
	boost::system::error_code m_error;

	DataMapper m_dataMapper;
};

template<typename T>
inline void Client::sendData(const std::vector<std::vector<T>>& data)
{
	sendDataGenerically(data);
}

template <typename T>
inline void Client::sendData(const std::vector<T>& data)
{
	sendDataGenerically(data);
}

template<typename T>
inline std::vector<T> Client::receiveData()
{
	boost::asio::streambuf receiveBuffer;
	m_error.clear();
	auto responseLength = read_until(m_socket, receiveBuffer, kReadLimit, m_error);

	std::vector<T> receivedData;
	if (m_error && m_error != boost::asio::error::eof)
	{
		std::cerr << "Data receiving failed: " << m_error.message() << '\n';
	}
	else
	{
		const char* data = boost::asio::buffer_cast<const char*>(receiveBuffer.data());
		receivedData = m_dataMapper.convertFromString<T>(data);
	}
	return receivedData;
}

template<typename T>
inline void Client::sendDataGenerically(const T & data)
{
	auto stringData = m_dataMapper.convertToString(data);
	m_error.clear();
	boost::asio::write(m_socket, boost::asio::buffer(stringData), m_error);
	if (!m_error)
	{
		std::cerr << "Data was sent. \n";
	}
	else
	{
		std::cerr << "Send failed: " << m_error.message() << '\n';
	}
}
