#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "BSMessage.h"
#include "Core/ThreadSafeQueue.h"
#include "Core/DataDistributor.h"

#include "asio.hpp"

#include <thread>
#include <mutex>
#include <atomic>

namespace BoxScore {

	class TCPServerConnection
	{
	public:
		using Ref = std::shared_ptr<TCPServerConnection>;

		TCPServerConnection(asio::io_context& context, asio::ip::tcp::socket socket);
		~TCPServerConnection();

		bool IsConnected() const { return m_socket.is_open(); }
		void Send(const BSMessage& message);

		void Disconnect();

	private:
		void WriteHeader();
		void WriteBody();

		asio::ip::tcp::socket m_socket;
		asio::io_context& m_contextRef;

		ThreadSafeQueue<BSMessage> m_queue;
	};

	/*
		TCPServer
		BoxScore specialized server implementation. First take at what is sure to evolve significantly as more testing is done
		Currently creates two threads, one for accepting clients, and one for posting data.

		PowerOn()/Shutdown() pair are use to initialize the server and start up the client accepting thread. This pair is essentially intended to go with
		application start and stop.

		StartDataFeed()/StopDataFeed() pair are used to tell the server when data is ready to be pulled from the ring buffer. This pari is intended to go with
		a data run start and stop.
	*/
	class TCPServer
	{
	public:
		TCPServer(uint16_t serverPort);
		~TCPServer();

		/*
		PowerOn()
		Starts up the acceptor thread of the server. This allows clients to begin to connect
		to our service. This should be done more or less once, at application initialization.
		*/
		void PowerOn();

		/*
		Shutdown()
		Shutdown the entire server, including both the acceptor thread and the data feed thread.
		This should be done essentially once, at application shutdown.
		*/
		void Shutdown();

		/*
		StartDataFeed()
		Starts the process to take data from the RingBuffer and feed it to clients (if they exist).
		This will be called many times, usually at the start of a new data taking run.
		*/
		void StartDataFeed();

		/*
		StopDataFeed()
		Stops the process to take data from the RingBuffer and feed it to clients (if they exist).
		This will be called many times, usually at the stop of a data taking run.
		*/
		void StopDataFeed();

		bool IsActive() { return m_acceptor.is_open(); }


	private:
		void MessageClients(const BSMessage& message);
		void WaitForClient();

		asio::io_context m_context;
		asio::ip::tcp::acceptor m_acceptor;

		std::mutex m_clientMutex;
		std::vector<TCPServerConnection::Ref> m_clients;

		DistributorClient m_dataHandle;

		std::thread m_acceptorThread;
		std::thread m_dataFeedThread;

		std::atomic<bool> m_isDataFeedRunning;
	};
}


#endif