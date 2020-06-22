#include <iostream>
#include <boost/asio.hpp>
#include <DataMappingTools.hpp>

#include <Client.h>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

int main()
{
	/////////////////////////////////////////////////////////////////
	//boost::asio::io_service io_service;
	////socket creation
	//tcp::socket socket(io_service);


	////connection
	//try
	//{
	//	socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 10000));

	//}
	//catch (const std::exception& e)
	//{
	//std::cerr<<e.what();
	//}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//// request/message from client
	//const string msg = "5.01023,7.31896,5.11042,2.31005,1.85523,3.26955,4.00249,2.98308,1.76224,2.16757,3.80629,4.7559,4.34738,3.4863,3.00205,2.98726,3.24807,3.47523,3.54092,3.43103,3.23336,3.17697,3.42309,3.75172,3.75172,3.42309,3.17697,3.23336,3.43103,3.54092,3.47523,3.24807,2.98726,3.00205,3.4863,4.34738,4.7559,3.80629,2.16757,1.76224,";
	//const string msg2 = "5.01023,7.31896,5.11042,2.31005,1.85523,3.26955,4.00249,2.98308,1.76224,2.16757,3.80629,4.7559,4.34738,3.4863,3.00205,2.98726,3.24807,3.47523,3.54092,3.43103,3.23336,3.17697,3.42309,3.75172,3.75172,3.42309,3.17697,3.23336,3.43103,3.54092,3.47523,3.24807,2.98726,3.00205,3.4863,4.34738,4.7559,3.80629,2.16757,1.76224";
	//boost::system::error_code error;

	std::vector<std::string> someValues;
	someValues.push_back("This code won't compile");

	std::vector<std::vector<double>> matrix;

	for (size_t i = 0; i < 10; i++)
	{
		std::vector<double> data;
		for (size_t j = 0; j < 40; j++)
		{
			data.push_back(j * 0.05);
		}
		matrix.push_back(data);
	}

	std::vector<double> values;
	for (int i = 0; i < 40; ++i)
	{
		values.push_back(i * 0.01);
	}

	Client client;
	client.connect("127.0.0.1", 10000);
	//client.sendData(someValues);

	auto x = client.receiveData();

	for (auto val : x)
		std::cout << val << ' ';


	//client.recieveData();
	//boost::asio::write(socket, boost::asio::buffer(msg + msg2), error);

	
	/////////////////////////////////////////////////////////////////
	//DataMapper mapper;

	/////////////////////////////////////////////////////////////////
	//// getting response from server
	//boost::asio::streambuf receive_buffer;
	//auto len = read_until(socket, receive_buffer, "\n", error);
	//if (error && error != boost::asio::error::eof)
	//{
	//	cout << "receive failed: " << error.message() << endl;
	//}
	//else
	//{
	//	const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
	//	auto convertedData = mapper.convertFromString<int>(data);
	//	cout << data << endl;
	//}
	///////////////////////////////////////////////////////////////
	return 0;
}

