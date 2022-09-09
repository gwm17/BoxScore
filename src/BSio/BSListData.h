#ifndef BS_LIST_DATA_H
#define BS_LIST_DATA_H

#include "DAQ/DigitizerDefs.h"

namespace BoxScore {

    //This is the subset of BSData that will be transferred via binary writes/reads
    //on either network or file io
	struct BSListData
	{
        uint16_t board;
        uint16_t channel;
        uint64_t timestamp;
        uint32_t energy;
        uint32_t energyShort;
        uint32_t flags;
    };

    /*
	Load BSData to a binary buffer(in our implementation this is as std::vector<char>),
    stripping away any wave data, returns index after last written to.
	Quite nasty. Writing structs is dangerous for cross-platformness, even with pod's
	Cannot guarantee padding is equivalent from different compilers, architectures.
	So, convert each individual data member to binary, and write our own compact message.
	*/
    static std::size_t LoadBSDataToBuffer(std::vector<char>& buffer, const BSData& data)
    {
		char* dataPointer;
		int loopIndex; // predeclare to save allocation time
		int position = buffer.empty() ? 0 : buffer.size() - 1;

		if (buffer.size() - position < Data::dataSize)
		{
			BS_ERROR("Could not load data to buffer; data size exceeded remaining buffer size {0}", buffer.size() - position);
			return position;
		}

		//Write board (2 bytes)
		dataPointer = (char*)&(data.board);
		for (loopIndex = 0; loopIndex < sizeof(data.board); loopIndex++)
			buffer[position++] = dataPointer[loopIndex];

		//Write channel (2 bytes)
		dataPointer = (char*)&(data.channel);
		for (loopIndex = 0; loopIndex < sizeof(data.channel); loopIndex++)
			buffer[position++] = dataPointer[loopIndex];

		//Write timestamp (8 bytes)
		dataPointer = (char*)&(data.timestamp);
		for (loopIndex = 0; loopIndex < sizeof(data.timestamp); loopIndex++)
			buffer[position++] = dataPointer[loopIndex];

		//Write energy (4 bytes)
		dataPointer = (char*)&(data.energy);
		for (loopIndex = 0; loopIndex < sizeof(data.energy); loopIndex++)
			buffer[position++] = dataPointer[loopIndex];

		//Write energyShort (4 bytes)
		dataPointer = (char*)&(data.energyShort);
		for (loopIndex = 0; loopIndex < sizeof(data.energyShort); loopIndex++)
			buffer[position++] = dataPointer[loopIndex];

		//Write flags (4 bytes)
		dataPointer = (char*)&(data.flags);
		for (loopIndex = 0; loopIndex < sizeof(data.flags); loopIndex++)
			buffer[position++] = dataPointer[loopIndex];

		return position;
    }

	/*
	Unload BSListData from a binary buffer(in our implementation this is as std::vector<char>),
	returns index after last pulled from. Writes to input parameter data
	Again, rather nasty. Here we C-style cast the data in the buffer to our types in the list data,
	write them to the input parameter reference, and then iterate forward the appropriate number of bytes.
	*/
	static std::size_t UnLoadBSListDataFromBuffer(const std::vector<char>& buffer, BSListData& data, std::size_t startPosition)
	{
		std::size_t position = startPosition;
		if (buffer.size() - startPosition < Data::dataSize)
		{
			BS_ERROR("Could not unload buffer to data; data size exceeded remaining buffer size {0}", buffer.size() - startPosition);
			return startPosition;
		}

		data.board =  *(uint16_t*)&(buffer[position]);
		position += 2;
		data.channel = *(uint16_t*)&(buffer[position]);
		position += 2;
		data.timestamp = *(uint16_t*)&(buffer[position]);
		position += 8;
		data.timestamp = *(uint16_t*)&(buffer[position]);
		position += 8;
		data.energy = *(uint32_t*)&(buffer[position]);
		position += 4;
		data.energyShort = *(uint32_t*)&(buffer[position]);
		position += 4;
		data.flags = *(uint32_t*)&(buffer[position]);
		position += 4;

		return position;
	}
}

#endif