#include "RingBuffer.h"

namespace BoxScore {

	RingBuffer* RingBuffer::s_instance = new RingBuffer();
	int RingBuffer::s_pushIndex = 0;
	int RingBuffer::s_inUseSize = 0;
	uint64_t RingBuffer::s_nextKey = 0;
	std::array<std::vector<BSData>, RingBuffer::s_bufferSize> RingBuffer::s_buffer;
	std::unordered_map<uint64_t, int> RingBuffer::s_consumerMap; //Maps key (id) to that consumer's popIndex
	std::mutex RingBuffer::s_bufferMutex;

	uint64_t RingBuffer::Attach()
	{
		std::scoped_lock<std::mutex> guard(s_bufferMutex);

		std::size_t consumerID = s_nextKey;
		s_consumerMap[consumerID] = s_pushIndex;
		++s_nextKey;
		return consumerID;
	}

	bool RingBuffer::Detach(uint64_t consumerID)
	{
		std::scoped_lock<std::mutex> guard(s_bufferMutex);

		if (s_consumerMap.find(consumerID) == s_consumerMap.end())
			return false;

		s_consumerMap.erase(consumerID);

		return true;
	}

	bool RingBuffer::PushData(const std::vector<BSData>& data)
	{
		std::scoped_lock<std::mutex> guard(s_bufferMutex);

		if (s_inUseSize == s_bufferSize)
			return false;

		s_buffer[s_pushIndex] = data;
		s_pushIndex = (s_pushIndex + 1) % s_bufferSize;
		++s_inUseSize;
		return true;
	}

	bool RingBuffer::PopData(uint64_t consumerID, std::vector<BSData>& localBuffer)
	{
		std::scoped_lock<std::mutex> guard(s_bufferMutex);

		auto iter = s_consumerMap.find(consumerID);
		if (s_inUseSize == 0 || iter->second == s_pushIndex || iter == s_consumerMap.end())
			return false;

		localBuffer = s_buffer[iter->second];
		if (CanReduce(consumerID))
			--s_inUseSize;

		++(iter->second);
		return true;
	}

	//Private, so lock should already be aquired
	bool RingBuffer::CanReduce(uint64_t consumerID)
	{
		int currentDistance = AbsoluteDistance(s_consumerMap[consumerID]);
		int distance;
		for (auto& iter : s_consumerMap)
		{
			if (iter.first == consumerID)
				continue;
			
			distance = AbsoluteDistance(s_consumerMap[iter.second]);
			if (distance >= currentDistance)
				return false;
		}

		return true;
	}

	int RingBuffer::AbsoluteDistance(int position)
	{
		int distance = s_pushIndex - position;
		return distance > 0 ? distance : s_bufferSize - (position + distance);
	}
}