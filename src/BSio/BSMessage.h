#ifndef BS_MESSAGE_H
#define BS_MESSAGE_H

namespace BoxScore {

	enum class BSMessageType
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
		BSHeader header;
		std::vector<char> body;

		std::size_t Size()
		{
			return header.size;
		}
	};

}