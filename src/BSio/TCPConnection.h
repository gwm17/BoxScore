#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "asio.hpp"

namespace BoxScore {

	enum class BSMessageType
	{
		List, //Maybe someday hahaha
		Mixed,
		None
	};

	struct BSHeader
	{
		BSMessageType type = BSMessageType::None;
		uint64_t size = 0;
	};

	class BSMessage
	{
	
	};

	class TCPConnection
	{
	public:

	private:

	};
}


#endif