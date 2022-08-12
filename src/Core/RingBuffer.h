#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "DAQ/DigitizerDefs.h"
#include <memory>
#include <mutex>

namespace BoxScore {

	//Singleton pattern. Always should be a ring buffer
	class RingBuffer
	{
	public:

		static uint64_t Attach(); // pointer owned by RingBuffer
		static bool Detach(uint64_t consumerID); //if true, consumer removed

		static bool PushData(const std::vector<BSData>& data);
		static bool PopData(uint64_t consumerID, std::vector<BSData>& localBuffer); //if true, copy data from ring buffer to local buffer

	private:
		RingBuffer() = default;
		static bool CanReduce(uint64_t consumerID);
		static int AbsoluteDistance(int position);
		static RingBuffer* s_instance;

		static constexpr int s_bufferSize = 10; //Idk if this is any good. Need to test more
		static std::array<std::vector<BSData>, s_bufferSize> s_buffer;
		static std::unordered_map<uint64_t, int> s_consumerMap; //Maps key (id) to that consumer's popIndex
		static int s_pushIndex;
		static int s_inUseSize;
		static uint64_t s_nextKey;

		static std::mutex s_bufferMutex; //Synchronization
	};
}

#endif