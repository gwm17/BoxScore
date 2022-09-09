#include "BSFile.h"
#include "BSListData.h"
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

	//See BSListData.h for details on loading data to buffer. 
	void BSFile::Write(const BSData& data)
	{
		m_currentLocation = LoadBSDataToBuffer(m_buffer, data);
		if (m_currentLocation == s_bufferSize)
			Flush();
	}
}