#include "BSFile.h"
#include <bit>

namespace BoxScore {

	BSFile::BSFile() :
		m_fileHandle(nullptr), m_currentLocation(0)
	{
		if (std::endian::native == std::endian::big)
			BS_WARN("You are using a big endian system! Data will be encoded in reverse byte order on most machines");
		m_buffer.resize(s_bufferSize);
	}

	BSFile::BSFile(const std::filesystem::path& path) :
		m_fileHandle(nullptr), m_currentLocation(0)
	{
		if (std::endian::native == std::endian::big)
			BS_WARN("You are using a big endian system! Data will be encoded in reverse byte order on most machines");
		m_buffer.resize(s_bufferSize);
		Open(path);
	}

	BSFile::~BSFile()
	{
		if (IsOpen())
			Close();
	}

	void BSFile::Open(const std::filesystem::path& path)
	{
		m_path = path;
		m_fileHandle = std::make_shared<std::ofstream>(path);
	}

	void BSFile::Close()
	{
		if (m_currentLocation != 0)
			Flush();
		m_fileHandle->close();
	}

	void BSFile::Flush()
	{
		m_fileHandle->write(m_buffer.data(), m_currentLocation);
		m_currentLocation = 0;
	}

	//Nasty work. Write bs data to temp buffer, which will later be flushed.
	//Avoid writing struct. Structs in general aren't easily portable (packing can be weird, and in general not cross platform)
	//We do not handle endianess here; we assume little endian for most portability
	void BSFile::Write(const BSData& data)
	{
		char* dataPointer;
		int loopIndex; // predeclare to save allocation time

		//Write board (2 bytes)
		dataPointer = (char*) &(data.board);
		for (loopIndex = 0; loopIndex < sizeof(data.board); loopIndex++)
			m_buffer[m_currentLocation++] = dataPointer[loopIndex];

		//Write channel (2 bytes)
		dataPointer = (char*) &(data.channel);
		for (loopIndex = 0; loopIndex < sizeof(data.channel); loopIndex++)
			m_buffer[m_currentLocation++] = dataPointer[loopIndex];
		
		//Write timestamp (8 bytes)
		dataPointer = (char*) &(data.timestamp);
		for (loopIndex = 0; loopIndex < sizeof(data.timestamp); loopIndex++)
			m_buffer[m_currentLocation++] = dataPointer[loopIndex];

		//Write energy (4 bytes)
		dataPointer = (char*) &(data.energy);
		for (loopIndex = 0; loopIndex < sizeof(data.energy); loopIndex++)
			m_buffer[m_currentLocation++] = dataPointer[loopIndex];

		//Write energyShort (4 bytes)
		dataPointer = (char*)&(data.energyShort);
		for (loopIndex = 0; loopIndex < sizeof(data.energyShort); loopIndex++)
			m_buffer[m_currentLocation++] = dataPointer[loopIndex];

		//Write flags (4 bytes)
		dataPointer = (char*)&(data.flags);
		for (loopIndex = 0; loopIndex < sizeof(data.flags); loopIndex++)
			m_buffer[m_currentLocation++] = dataPointer[loopIndex];

		if (m_currentLocation == s_bufferSize)
			Flush();
	}
}