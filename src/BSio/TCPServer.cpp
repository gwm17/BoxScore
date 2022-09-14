#include "TCPServer.h"

namespace BoxScore {

	TCPServerConnection::TCPServerConnection(asio::io_context& context, asio::ip::tcp::socket socket) :
		m_contextRef(context), m_socket(std::move(socket))
	{
	}

	TCPServerConnection::~TCPServerConnection()
	{
	}

	/*
		The job we push to asio is slightly strange. If the queue is empty when we try to push a new job to it, it tells us
		that the loop of write calls and job submissions had stopped (or was not yet started), so we need to restart/start the 
		cycle of writing data to the socket.
	*/
	void TCPServerConnection::Send(const BSMessage& message)
	{
		asio::post(m_contextRef,
			[this, message]()
			{
				bool wasEmpty = m_queue.IsEmpty();
				m_queue.PushBack(message);
				if (wasEmpty)
				{
					WriteHeader();
				}
			});
	}

	void TCPServerConnection::Disconnect()
	{
		if (IsConnected())
			asio::post(m_contextRef, [this]() { m_socket.close(); });
	}

	void TCPServerConnection::WriteHeader()
	{
		std::vector<char> headerData = m_queue.Front().GetHeaderRaw();
		asio::async_write(m_socket, asio::buffer(headerData, headerData.size()),
			[this](std::error_code ec, std::size_t length) //Callback upon completion
			{
				if (!ec)
				{
					if (m_queue.Front().Size() > 0)
						WriteBody(); //submit next job to asio: write body data
					else
					{
						m_queue.PopFront();
						if (!m_queue.IsEmpty())
							WriteHeader(); //submit next job to asio: write the next message
					}
				}
				else
				{
					BS_WARN("TCPServerConnection -> Failure to write header of message, with error code: {0}", ec.message());
					BS_WARN("Closing connection...");
					m_socket.close();
				}
			});
	}

	void TCPServerConnection::WriteBody()
	{
		asio::async_write(m_socket, asio::buffer(m_queue.Front().body, m_queue.Front().Size()),
			[this](std::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					m_queue.PopFront();
					if (!m_queue.IsEmpty())
						WriteHeader();
				}
				else
				{
					BS_WARN("TCPServerConnection -> Failure to write body of message, with error code: {0}", ec.message());
					BS_WARN("Closing connection...");
					m_socket.close();
				}
			});
	}

	TCPServer::TCPServer(uint16_t serverPort) :
		m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), serverPort)), m_isDataFeedRunning(false)
	{

	}

	TCPServer::~TCPServer()
	{
	}

	void TCPServer::PowerOn()
	{
		try
		{
			WaitForClient();
			m_acceptorThread = std::thread([this]() { m_context.run(); });
		}
		catch (std::exception& e)
		{
			BS_ERROR("Server caught exception: {0}", e.what());
			return;
		}

		BS_INFO("Server started and waiting for client connection");
	}

	void TCPServer::Shutdown()
	{
		StopDataFeed();

		m_context.stop();
		if (m_acceptorThread.joinable())
			m_acceptorThread.join();

		BS_INFO("Sever has been shutdown");
	}

	void TCPServer::StartDataFeed()
	{
		if (m_dataFeedThread.joinable())
		{
			BS_WARN("Server malfunction! You forgot to call StopDataFeed before calling StartDataFeed! Trying to recover...");
			StopDataFeed();
		}

		if (m_dataHandle.dataQueue != nullptr)
			m_dataHandle = DataDistributor::Connect();

		m_isDataFeedRunning = true;
		m_dataFeedThread = std::thread([this]()
			{
				while (m_isDataFeedRunning)
				{
					m_dataHandle.dataQueue->Wait();

					if (!m_dataHandle.dataQueue->IsEmpty())
						continue;

					BSMessage message(m_dataHandle.dataQueue->Front());
					MessageClients(message);
					m_dataHandle.dataQueue->PopFront();
				}
			});
	}

	void TCPServer::StopDataFeed()
	{
		m_isDataFeedRunning = false;
		m_dataHandle.dataQueue->ForceWakeup();
		if (m_dataFeedThread.joinable())
			m_dataFeedThread.join();
		m_dataHandle.dataQueue->ResetWakeup();
	}

	void TCPServer::MessageClients(const BSMessage& message)
	{
		bool isAnyClientInvalid = false;
		std::scoped_lock<std::mutex> guard(m_clientMutex);

		for (auto& client : m_clients)
		{
			if (client->IsConnected())
			{
				client->Send(message);
			}
			else
			{
				client->Disconnect();
				isAnyClientInvalid = true;
			}
		}

		if (isAnyClientInvalid)
			m_clients.erase(std::remove_if(m_clients.begin(), m_clients.end(), [](TCPServerConnection::Ref& client) { return client->IsConnected(); }), m_clients.end());
	}

	void TCPServer::WaitForClient()
	{
		m_acceptor.async_accept(
			[this](std::error_code ec, asio::ip::tcp::socket socket)
			{
				if (!ec)
				{
					BS_INFO("Server connection to new client: {0}", socket.remote_endpoint());
					{
						//TCPServerConnection::Ref newClient = std::make_shared<TCPServerConnection>(m_context, std::move(socket));
						std::scoped_lock<std::mutex> guard(m_clientMutex);
						//m_clients.push_back(newClient);
						m_clients.push_back(std::make_shared<TCPServerConnection>(m_context, std::move(socket)));
					}
				}
				else
					BS_ERROR("Server Connection Failure with error code: {0}", ec.message());
			});
	}
}