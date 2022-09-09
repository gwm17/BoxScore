#ifndef BS_MESSAGE_H
#define BS_MESSAGE_H

#include "DAQ/DigitizerDefs.h"
#include "BSListData.h" 

namespace BoxScore {

	enum class BSMessageType : uint64_t
	{
		List,
		Mixed, //Maybe someday hahaha
		None
	};

	struct BSHeader
	{
		BSMessageType type = BSMessageType::None;
		uint64_t size = 0;
	};

	struct BSMessage
	{
		BSMessage() = default;
		BSMessage(const std::vector<BSData>& data)
		{
			header.type = BSMessageType::List;
			for (const auto& datum : data)
				LoadBSDataToBuffer(body, datum);
		}

		BSHeader header;
		std::vector<char> body;

		std::size_t Size() const
		{
			return header.size;
		}

		//Nasty work. Convert header into a raw bytes array for transmission. Makes it so padding is no longer a concern.
		std::vector<char> GetHeaderRaw() const
		{
			std::vector<char> rawBytes(sizeof(header.type) + sizeof(header.size));
			std::size_t position = 0;
			int loopIndex;
			
			char* dataPointer = (char*)&header.type;
			for (loopIndex = 0; loopIndex < sizeof(header.type); loopIndex++)
				rawBytes[position++] = dataPointer[loopIndex];
			
			dataPointer = (char*)&header.size;
			for (loopIndex = 0; loopIndex < sizeof(header.size); loopIndex++)
				rawBytes[position++] = dataPointer[loopIndex];

			return rawBytes;
		}
	};

}

#endif