#include "TCPServer.h"

namespace BoxScore {

	TCPServerConnection::TCPServerConnection(asio::io_context& context, asio::ip::tcp::socket socket) :
		m_contextRef(context), m_socket(std::move(socket))
	{
	}

	TCPServerConnection::~TCPServerConnection()
	{
	}
}