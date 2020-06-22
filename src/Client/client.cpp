#include "Client.h"

#include <iostream>

using boost::asio::ip::tcp;

Client::Client()
	: m_ioService(),
	m_socket(m_ioService),
	m_dataMapper()
{
}

void Client::connect(std::string_view ipAddress, int port)
{
	try
	{
		m_socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(ipAddress.data()), port));
	}
	catch (const boost::system::system_error& e)
	{
		std::cerr << e.what() << '\n';
	}
}
