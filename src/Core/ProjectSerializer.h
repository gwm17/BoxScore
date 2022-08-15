#ifndef PROJECT_SERIALIZER_H
#define PROJECT_SERIALIZER_H

#include "BSProject.h"

#include <filesystem>

namespace BoxScore {

	class ProjectSerializer
	{
	public:
		ProjectSerializer(const std::filesystem::path& path);
		~ProjectSerializer();
		
		void SerializeData(const BSProject::Ref& project);
		void DeserializeData(const BSProject::Ref& project);

	private:
		std::filesystem::path m_filepath;

	};
}

#endif