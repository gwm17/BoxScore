#ifndef SCALAR_DISTRIBUTOR_H
#define SCALAR_DISTRIBUTOR_H

#include <atomic>

namespace BoxScore {

	//Scalars can be incremented thread-safe, lock-free using atomics
	struct Scalar
	{
		using Ref = std::shared_ptr<Scalar>;
		Scalar(const std::string& n) :
			value(0), lastCheckedValue(0), name(n)
		{
		}

		std::atomic<uint64_t> value;
		std::atomic<uint64_t> lastCheckedValue; //Value from the last time GetRate was called
		std::atomic<float> rate;
		std::string name;
	};

	class ScalarDistributor
	{
	public:

		//Bind/UnBind is NOT thread-safe. Should not be used in a multi-threaded context.
		static const Scalar::Ref& Bind(const std::string& name);
		static void UnBind(const std::string& name);

		//CalculateRates/GetRate is implicitly thread safe as it does not modify the map itself
		static void CalculateRates(double timestep);
		static float GetRate(const std::string& name); //Returns in Hz; timestep should be in seconds
		//ClearScalars is implicitly thread safe as it does not modify the map itself
		static void ClearScalars();

	private:
		static std::unordered_map<std::string, Scalar::Ref> s_scalarMap;
	};

}

#endif