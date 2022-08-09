#ifndef PROJECT_SERIALIZER_H
#define PROJECT_SERIALIZER_H

#include "BSProject.h"

namespace BoxScore {

	class ProjectSerializer
	{
	public:
		ProjectSerializer(const std::string& filepath);
		~ProjectSerializer();
		
		void SerializeData(const BSProject::Ref& project);
		void DeserializeData(const BSProject::Ref& project);

	private:
		std::string m_filepath;
	};
}

#endif