/*
	DataDistributor.h
	Thread-safe distribution of data to multpile clients. Each client must register using the Connect() function, and de-register using Disconnect().
	A client is given a DistributorClient struct which contains the id for that client as well as a raw pointer to the dataQueue allocated for that client.
	Use raw pointers to emphasize that memory is not automatically managed, and the Connect()/Disconnect() idiom MUST be used.

	This is a singleton, there must always be one and only one DataDistributor

	DataDistributor itself is not thread-safe. All connect/disconnect calls should be done from the main application thread. PushData / queue accesss is thread safe,
	as the queues are internally thread-safe (as the name implies)
*/
#ifndef DATA_DISTRIBUTOR_H
#define DATA_DISTRIBUTOR_H

#include "ThreadSafeQueue.h"
#include "DAQ/DigitizerDefs.h"


namespace BoxScore {

	struct DistributorClient
	{
		uint64_t id = -1; //max val, illegal value 
		ThreadSafeQueue<std::vector<BSData>>* dataQueue = nullptr;
	};

	class DataDistributor
	{
	public:

		static void PushData(const std::vector<BSData>& data);

		static DistributorClient Connect();
		static void Disconnect(DistributorClient& client);

	private:
		static std::unordered_map<uint64_t, ThreadSafeQueue<std::vector<BSData>>*> s_clientMap;
		static uint64_t s_nextClientID;
	};
}
#endif