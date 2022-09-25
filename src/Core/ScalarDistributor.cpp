#include "ScalarDistributor.h"

namespace BoxScore {

	std::unordered_map<std::string, Scalar::Ref> ScalarDistributor::s_scalarMap;

	const Scalar::Ref& ScalarDistributor::Bind(const std::string& name)
	{
		auto iter = s_scalarMap.find(name);
		if (iter != s_scalarMap.end())
			return iter->second;
		else
		{
			s_scalarMap[name] = std::make_shared<Scalar>(name);
			return s_scalarMap[name];
		}
	}

	void ScalarDistributor::UnBind(const std::string& name)
	{
		if(s_scalarMap.contains(name))
			s_scalarMap.erase(name);
	}

	void ScalarDistributor::CalculateRates(double timestep)
	{
		for(auto& scalar : s_scalarMap)
		{
			uint64_t changeInCounts = scalar.second->value - scalar.second->lastCheckedValue;
			scalar.second->lastCheckedValue += changeInCounts;
			scalar.second->rate = changeInCounts / timestep;
		}
	}

	float ScalarDistributor::GetRate(const std::string& name)
	{
		auto iter = s_scalarMap.find(name);
		if (iter != s_scalarMap.end())
			return iter->second->rate;
		else
			return 0.0f;
	}

	void ScalarDistributor::ClearScalars()
	{
		for (auto& scalar : s_scalarMap)
		{
			scalar.second->value = 0.0f;
			scalar.second->lastCheckedValue = 0.0f;
		}
	}

}