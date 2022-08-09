#include "ProjectSerializer.h"

#include <fstream>
#include "yaml-cpp/yaml.h"


namespace YAML {

	template<>
	struct convert<BoxScore::DigitizerArgs>
	{
		static Node encode(const BoxScore::DigitizerArgs& args)
		{
			Node node(NodeType::Map);
			node.force_insert<std::string, int>("ConnectionType", args.type);
			node.force_insert<std::string, int>("LinkNumber", args.linkNumber);
			node.force_insert<std::string, int>("CONETNode", args.conetNode);
			node.force_insert<std::string, uint32_t>("VMEAddress", args.vmeAddress);
			node.force_insert<std::string, int>("Handle", args.handle);
			node.force_insert<std::string, int>("Model", args.model);
			node.force_insert<std::string, int>("Firmware", args.firmware);
			node.force_insert<std::string, int>("Channels", args.channels);

			return node;
		}

		static bool decode(const Node& node, BoxScore::DigitizerArgs& args)
		{
			if (!node.IsMap())
				return false;

			args.type = (CAEN_DGTZ_ConnectionType) node["ConnectionType"].as<int>();
			args.linkNumber = node["LinkNumber"].as<int>();
			args.conetNode = node["ConetNode"].as<int>();
			args.vmeAddress = node["VMEAddress"].as<uint32_t>();
			args.handle = node["Handle"].as<int>();
			args.model = (CAEN_DGTZ_BoardModel_t) node["Model"].as<int>();
			args.firmware = (CAEN_DGTZ_DPPFirmware_t) node["Firmware"].as<int>();
			args.channels = node["Channels"].as<int>();

			return true;
		}
	};

	template<>
	struct convert<BoxScore::DigitizerParameters>
	{
		static Node encode(const BoxScore::DigitizerParameters& params)
		{
			Node node(NodeType::Map);
			node.force_insert<std::string, int>("RecordLength", params.recordLength);
			node.force_insert<std::string, uint32_t>("ChannelMask", params.channelMask);
			node.force_insert<std::string, int>("EventAggr", params.eventAggr);
			node.force_insert<std::string, int>("AcqMode", params.acqMode);
			node.force_insert<std::string, int>("DPPAcqMode", params.dppAcqMode);
			node.force_insert<std::string, int>("IOLevel", params.IOlevel);
			node.force_insert<std::string, int>("TriggerMode", params.triggerMode);
			node.force_insert<std::string, int>("SyncMode", params.syncMode);
		}

		static bool decode(const Node& node, BoxScore::DigitizerParameters& params)
		{
			if (!node.IsMap())
				return false;

			params.recordLength = node["RecordLength"].as<uint32_t>();
			params.channelMask = node["ChannelMask"].as<uint32_t>();
			params.eventAggr = node["EventAggr"].as<int>();
			params.acqMode = (CAEN_DGTZ_AcqMode_t)node["AcqMode"].as<int>();
			params.dppAcqMode = (CAEN_DGTZ_DPP_AcqMode_t)node["DPPAcqMode"].as<int>();
			params.IOlevel = (CAEN_DGTZ_IOLevel_t)node["IOLevel"].as<int>();
			params.triggerMode = (CAEN_DGTZ_TriggerMode_t)node["TriggerMode"].as<int>();
			params.syncMode = (CAEN_DGTZ_RunSyncMode_t)node["SyncMode"].as<int>();

			return true;
		}
	};

	template<>
	struct convert<BoxScore::PHAParameters>
	{
		static Node encode(const BoxScore::PHAParameters& params)
		{
			Node node(NodeType::Map);
			node.force_insert<std::string, bool>("IsEnabled", params.isEnabled);
			node.force_insert<std::string, uint32_t>("PreTrigger", params.preTriggerSamples);
			node.force_insert<std::string, float>("DCOffset", params.dcOffset);
			node.force_insert<std::string, int>("PulsePolarity", params.pulsePolarity);
			node.force_insert<std::string, int>("DynamicRange", params.dynamicRange);
			node.force_insert<std::string, int>("DecayTimeConst", params.decayTimeConst);
			node.force_insert<std::string, int>("TrapFlatTop", params.trapFlatTop);
			node.force_insert<std::string, int>("TrapRiseTime", params.trapRiseTime);
			node.force_insert<std::string, int>("FlatTopDelay",  params.flatTopDelay);
			node.force_insert<std::string, int>("TriggerSmoothing", params.triggerFilterSmoothing);
			node.force_insert<std::string, int>("InputRiseTime", params.inputRiseTime);
			node.force_insert<std::string, int>("TriggerThreshold", params.triggerThreshold);
			node.force_insert<std::string, int>("SamplesBaseline", params.samplesBaseLineMean);
			node.force_insert<std::string, int>("SamplesPeak", params.samplesPeakMean);
			node.force_insert<std::string, int>("PeakHoldoff", params.peakHoldOff);
			node.force_insert<std::string, int>("BaselineHoldoff", params.baseLineHoldOff);
			node.force_insert<std::string, int>("TriggerHoldoff", params.triggerHoldOff);
			node.force_insert<std::string, int>("RiseTimeWindow", params.riseTimeValidationWindow);
			node.force_insert<std::string, int>("RiseTimeDiscrimination", params.riseTimeDiscrimination);
			node.force_insert<std::string, int>("DigitalProbeGain", params.digitalProbeGain);
			node.force_insert<std::string, int>("EnergyNormFactor", params.energyNormalizationFactor);
			node.force_insert<std::string, int>("InputDecimation", params.inputDecimation);

			return node;
		}

		static bool decode(const Node& node, BoxScore::PHAParameters& params)
		{
			if (!node.IsMap())
				return false;

			params.isEnabled = node["IsEnabled"].as<bool>();
			params.preTriggerSamples = node["PreTrigger"].as<uint32_t>();
			params.dcOffset = node["DCOffset"].as<float>();
			params.pulsePolarity = (CAEN_DGTZ_PulsePolarity_t)node["PulsePolarity"].as<int>();
			params.dynamicRange = (BoxScore::DynamicRange)node["DynamicRange"].as<int>();
			params.decayTimeConst = node["DecayTimeConst"].as<int>();
			params.trapFlatTop = node["TrapFlatTop"].as<int>();
			params.trapRiseTime = node["TrapRiseTime"].as<int>();
			params.flatTopDelay = node["FlatTopDelay"].as<int>();
			params.triggerFilterSmoothing = node["TriggerSmoothing"].as<int>();
			params.inputRiseTime = node["InputRiseTime"].as<int>();
			params.triggerThreshold = node["TriggerThreshold"].as<int>();
			params.samplesBaseLineMean = node["SamplesBaseline"].as<int>();
			params.samplesPeakMean = node["SamplesPeak"].as<int>();
			params.peakHoldOff = node["PeakHoldoff"].as<int>();
			params.baseLineHoldOff = node["BaselineHoldoff"].as<int>();
			params.triggerHoldOff = node["TriggerHoldoff"].as<int>();
			params.riseTimeValidationWindow = node["RiseTimeWindow"].as<int>();
			params.riseTimeDiscrimination = node["RiseTimeDiscrimination"].as<int>();
			params.digitalProbeGain = node["DigitalProbeGain"].as<int>();
			params.energyNormalizationFactor = node["EnergyNormFactor"].as<int>();
			params.inputDecimation = node["InputDecimation"].as<int>();

			return true;
		}
	};

	template<>
	struct convert<BoxScore::PSDParameters>
	{
		static Node encode(const BoxScore::PSDParameters& params)
		{
			Node node(NodeType::Map);

			node.force_insert<std::string, bool>("IsEnabled", params.isEnabled);
			node.force_insert<std::string, uint32_t>("PreTrigger", params.preTriggerSamples);
			node.force_insert<std::string, float>("DCOffset", params.dcOffset);
			node.force_insert<std::string, int>("PulsePolarity", params.pulsePolarity);
			node.force_insert<std::string, int>("DynamicRange", params.dynamicRange);
			node.force_insert<std::string, int>("BaselineThreshold", params.baselineThreshold);
			node.force_insert<std::string, int>("TriggerHoldoff", params.triggerHoldOff);
			node.force_insert<std::string, int>("TriggerThreshold", params.triggerThreshold);
			node.force_insert<std::string, int>("SelfTrigger", params.selfTrigger);
			node.force_insert<std::string, int>("ChargeSensitivity", params.chargeSensitivity);
			node.force_insert<std::string, int>("ShortGate", params.shortGate);
			node.force_insert<std::string, int>("LongGate", params.longGate);
			node.force_insert<std::string, int>("PreGate", params.preGate);
			node.force_insert<std::string, int>("TriggerWindow", params.triggerValidationWindow);
			node.force_insert<std::string, int>("BaselineSamples", params.samplesBasline);
			node.force_insert<std::string, int>("DiscriminatorType", params.discrminatorType);
			node.force_insert<std::string, int>("CFDFraction", params.cfdFraction);
			node.force_insert<std::string, int>("CFDDelay", params.cfdDelay);
			node.force_insert<std::string, int>("TriggerConfig", params.triggerConfig);
			node.force_insert<std::string, int>("PileUpRejection", params.pileUpRejection);
			node.force_insert<std::string, int>("PURGap", params.purgap);

			return node;
		}

		static bool decode(const Node& node, BoxScore::PSDParameters& params)
		{
			if (!node.IsMap())
				return false;

			params.isEnabled = node["IsEnabled"].as<bool>();
			params.preTriggerSamples = node["PreTrigger"].as<int>();
			params.dcOffset = node["DCOffset"].as<float>();
			params.pulsePolarity = (CAEN_DGTZ_PulsePolarity_t)node["PulsePolarity"].as<int>();
			params.dynamicRange = (BoxScore::DynamicRange)node["DynamicRange"].as<int>();
			params.baselineThreshold = node["BaselineThreshold"].as<int>();
			params.triggerHoldOff = node["TriggerHoldoff"].as<int>();
			params.triggerThreshold = node["TriggerThreshold"].as<int>();
			params.selfTrigger = node["SelfTrigger"].as<int>();
			params.chargeSensitivity = node["ChargeSensitivity"].as<int>();
			params.shortGate = node["ShortGate"].as<int>();
			params.longGate = node["LongGate"].as<int>();
			params.preGate = node["PreGate"].as<int>();
			params.triggerValidationWindow = node["TriggerWindow"].as<int>();
			params.samplesBasline = node["BaselineSamples"].as<int>();
			params.discrminatorType = node["DiscriminatorType"].as<int>();
			params.cfdFraction = node["CFDFraction"].as<int>();
			params.cfdDelay = node["CFDDelay"].as<int>();
			params.triggerConfig = (CAEN_DGTZ_DPP_TriggerConfig_t)node["TriggerConfig"].as<int>();
			params.pileUpRejection = (CAEN_DGTZ_DPP_PUR_t)node["PileUpRejection"].as<int>();
			params.purgap = node["PURGap"].as<int>();

			return true;
		}
	};

	template<>
	struct convert<BoxScore::PHAWaveParameters>
	{
		static Node encode(const BoxScore::PHAWaveParameters& waves)
		{
			Node node(NodeType::Map);

			node.force_insert<std::string, int>("IsDual", waves.isDual);
			node.force_insert<std::string, int>("AnalogProbe1", waves.analogProbe1);
			node.force_insert<std::string, int>("AnalogProbe2", waves.analogProbe2);
			node.force_insert<std::string, int>("DigitalProbe1", waves.digitalProbe1);

			return node;
		}

		static bool decode(const Node& node, BoxScore::PHAWaveParameters& waves)
		{
			if (!node.IsMap())
				return false;

			waves.isDual = (CAEN_DGTZ_DPP_VirtualProbe_t)node["IsDual"].as<int>();
			waves.analogProbe1 = (BoxScore::PHAVirtualProbe1Options)node["AnalogProbe1"].as<int>();
			waves.analogProbe2 = (BoxScore::PHAVirtualProbe2Options)node["AnalogProbe2"].as<int>();
			waves.digitalProbe1 = (BoxScore::PHADigitalProbe1Options)node["DigitalProbe1"].as<int>();

			return true;
		}
	};

	template<>
	struct convert<BoxScore::PSDWaveParameters>
	{
		static Node encode(const BoxScore::PSDWaveParameters& waves)
		{
			Node node(NodeType::Map);

			node.force_insert<std::string, int>("IsDual", waves.isDual);
			node.force_insert<std::string, int>("AnalogProbe1", waves.analogProbe1);
			node.force_insert<std::string, int>("AnalogProbe2", waves.analogProbe2);
			node.force_insert<std::string, int>("DigitalProbe1", waves.digitalProbe1);
			node.force_insert<std::string, int>("DigitalProbe2", waves.digitalProbe2);

			return node;
		}

		static bool decode(const Node& node, BoxScore::PSDWaveParameters& waves)
		{
			if (!node.IsMap())
				return false;

			waves.isDual = (CAEN_DGTZ_DPP_VirtualProbe_t)node["IsDual"].as<int>();
			waves.analogProbe1 = (BoxScore::PSDVirtualProbe1Options)node["AnalogProbe1"].as<int>();
			waves.analogProbe2 = (BoxScore::PSDVirtualProbe2Options)node["AnalogProbe2"].as<int>();
			waves.digitalProbe1 = (BoxScore::PSDDigitalProbe1Options)node["DigitalProbe1"].as<int>();
			waves.digitalProbe2 = (BoxScore::PSDDigitalProbe2Options)node["DigitalProbe2"].as<int>();

			return true;
		}
	};
	
	Emitter& operator<<(Emitter& stream, const BoxScore::DigitizerArgs& args)
	{
		stream << YAML::BeginMap;
		stream << YAML::Key << "ConnectionType" << YAML::Value << args.type;
		stream << YAML::Key << "LinkNumber" << YAML::Value << args.linkNumber;
		stream << YAML::Key << "CONETNode" << YAML::Value << args.conetNode;
		stream << YAML::Key << "VMEAddress" << YAML::Value << args.vmeAddress;
		stream << YAML::Key << "Handle" << YAML::Value << args.handle;
		stream << YAML::Key << "Model" << YAML::Value << args.model;
		stream << YAML::Key << "Firmware" << YAML::Value << args.firmware;
		stream << YAML::Key << "Channels" << YAML::Value << args.channels;
		stream << YAML::EndMap;

		return stream;
	}

	Emitter& operator<<(Emitter& stream, const BoxScore::DigitizerParameters& params)
	{
		stream << YAML::BeginMap;
		stream << YAML::Key << "RecordLength" << YAML::Value << params.recordLength;
		stream << YAML::Key << "ChannelMask" << YAML::Value << params.channelMask;
		stream << YAML::Key << "EventAggr" << YAML::Value << params.eventAggr;
		stream << YAML::Key << "AcqMode" << YAML::Value << params.acqMode;
		stream << YAML::Key << "DPPAcqMode" << YAML::Value << params.dppAcqMode;
		stream << YAML::Key << "IOLevel" << YAML::Value << params.IOlevel;
		stream << YAML::Key << "TriggerMode" << YAML::Value << params.triggerMode;
		stream << YAML::Key << "SyncMode" << YAML::Value << params.syncMode;
		stream << YAML::EndMap;
		
		return stream;
	}

	Emitter& operator<<(Emitter& stream, const BoxScore::PHAParameters& params)
	{
		stream << YAML::BeginMap;
		stream << YAML::Key << "IsEnabled" << YAML::Value << params.isEnabled;
		stream << YAML::Key << "PreTrigger" << YAML::Value << params.preTriggerSamples;
		stream << YAML::Key << "DCOffset" << YAML::Value << params.dcOffset;
		stream << YAML::Key << "PulsePolarity" << YAML::Value << params.pulsePolarity;
		stream << YAML::Key << "DynamicRange" << YAML::Value << params.dynamicRange;
		stream << YAML::Key << "DecayTimeConst" << YAML::Value << params.decayTimeConst;
		stream << YAML::Key << "TrapFlatTop" << YAML::Value << params.trapFlatTop;
		stream << YAML::Key << "TrapRiseTime" << YAML::Value << params.trapRiseTime;
		stream << YAML::Key << "FlatTopDelay" << YAML::Value << params.flatTopDelay;
		stream << YAML::Key << "TriggerSmoothing" << YAML::Value << params.triggerFilterSmoothing;
		stream << YAML::Key << "InputRiseTime" << YAML::Value << params.inputRiseTime;
		stream << YAML::Key << "TriggerThreshold" << YAML::Value << params.triggerThreshold;
		stream << YAML::Key << "SamplesBaseline" << YAML::Value << params.samplesBaseLineMean;
		stream << YAML::Key << "SamplesPeak" << YAML::Value << params.samplesPeakMean;
		stream << YAML::Key << "PeakHoldoff" << YAML::Value << params.peakHoldOff;
		stream << YAML::Key << "BaselineHoldoff" << YAML::Value << params.baseLineHoldOff;
		stream << YAML::Key << "TriggerHoldoff" << YAML::Value << params.triggerHoldOff;
		stream << YAML::Key << "RiseTimeWindow" << YAML::Value << params.riseTimeValidationWindow;
		stream << YAML::Key << "RiseTimeDiscrimination" << YAML::Value << params.riseTimeDiscrimination;
		stream << YAML::Key << "DigitalProbeGain" << YAML::Value << params.digitalProbeGain;
		stream << YAML::Key << "EnergyNormFactor" << YAML::Value << params.energyNormalizationFactor;
		stream << YAML::Key << "InputDecimation" << YAML::Value << params.inputDecimation;
		stream << YAML::EndMap;

		return stream;
	}

	Emitter& operator<<(Emitter& stream, const BoxScore::PSDParameters& params)
	{
		stream << YAML::BeginMap;
		stream << YAML::Key << "IsEnabled" << YAML::Value << params.isEnabled;
		stream << YAML::Key << "PreTrigger" << YAML::Value << params.preTriggerSamples;
		stream << YAML::Key << "DCOffset" << YAML::Value << params.dcOffset;
		stream << YAML::Key << "PulsePolarity" << YAML::Value << params.pulsePolarity;
		stream << YAML::Key << "DynamicRange" << YAML::Value << params.dynamicRange;
		stream << YAML::Key << "BaselineThreshold" << YAML::Value << params.baselineThreshold;
		stream << YAML::Key << "TriggerHoldoff" << YAML::Value << params.triggerHoldOff;
		stream << YAML::Key << "TriggerThreshold" << YAML::Value << params.triggerThreshold;
		stream << YAML::Key << "SelfTrigger" << YAML::Value << params.selfTrigger;
		stream << YAML::Key << "ChargeSensitivity" << YAML::Value << params.chargeSensitivity;
		stream << YAML::Key << "ShortGate" << YAML::Value << params.shortGate;
		stream << YAML::Key << "LongGate" << YAML::Value << params.longGate;
		stream << YAML::Key << "PreGate" << YAML::Value << params.preGate;
		stream << YAML::Key << "TriggerWindow" << YAML::Value << params.triggerValidationWindow;
		stream << YAML::Key << "BaselineSamples" << YAML::Value << params.samplesBasline;
		stream << YAML::Key << "DiscriminatorType" << YAML::Value << params.discrminatorType;
		stream << YAML::Key << "CFDFraction" << YAML::Value << params.cfdFraction;
		stream << YAML::Key << "CFDDelay" << YAML::Value << params.cfdDelay;
		stream << YAML::Key << "TriggerConfig" << YAML::Value << params.triggerConfig;
		stream << YAML::Key << "PileUpRejection" << YAML::Value << params.pileUpRejection;
		stream << YAML::Key << "PURGap" << YAML::Value << params.purgap;
		stream << YAML::EndMap;

		return stream;
	}

	Emitter& operator<<(Emitter& stream, const std::vector<BoxScore::PHAParameters>& channels)
	{
		stream << YAML::BeginSeq;
		for (const auto& params : channels)
			stream << params;
		stream << YAML::EndSeq;
		return stream;
	}

	Emitter& operator<<(Emitter& stream, const std::vector<BoxScore::PSDParameters>& channels)
	{
		stream << YAML::BeginSeq;
		for (const auto& params : channels)
			stream << params;
		stream << YAML::EndSeq;
		return stream;
	}

	Emitter& operator<<(Emitter& stream, const BoxScore::PHAWaveParameters& waves)
	{
		stream << YAML::BeginMap;
		stream << YAML::Key << "IsDual" << YAML::Value << waves.isDual;
		stream << YAML::Key << "AnalogProbe1" << YAML::Value << waves.analogProbe1;
		stream << YAML::Key << "AnalogProbe2" << YAML::Value << waves.analogProbe1;
		stream << YAML::Key << "DigitalProbe1" << YAML::Value << waves.digitalProbe1;
		stream << YAML::EndMap;

		return stream;
	}

	Emitter& operator<<(Emitter& stream, const BoxScore::PSDWaveParameters& waves)
	{
		stream << YAML::BeginMap;
		stream << YAML::Key << "IsDual" << YAML::Value << waves.isDual;
		stream << YAML::Key << "AnalogProbe1" << YAML::Value << waves.analogProbe1;
		stream << YAML::Key << "AnalogProbe2" << YAML::Value << waves.analogProbe1;
		stream << YAML::Key << "DigitalProbe1" << YAML::Value << waves.digitalProbe1;
		stream << YAML::Key << "DigitalProbe2" << YAML::Value << waves.digitalProbe2;
		stream << YAML::EndMap;

		return stream;
	}
	
}

namespace BoxScore {

	/*
	//Serialize funcs
	static void SerializeDigitizerArgs(YAML::Emitter& stream, const DigitizerArgs& args)
	{
		stream << YAML::Key << "DigitizerArgs" << YAML::Value << YAML::BeginMap;
		stream << YAML::Key << "ConnectionType" << YAML::Value << args.type;
		stream << YAML::Key << "LinkNumber" << YAML::Value << args.linkNumber;
		stream << YAML::Key << "CONETNode" << YAML::Value << args.conetNode;
		stream << YAML::Key << "VMEAddress" << YAML::Value << args.vmeAddress;
		stream << YAML::Key << "Handle" << YAML::Value << args.handle;
		stream << YAML::Key << "Model" << YAML::Value << args.model;
		stream << YAML::Key << "Firmware" << YAML::Value << args.firmware;
		stream << YAML::Key << "Channels" << YAML::Value << args.channels;
		stream << YAML::EndMap;
	}

	static void SerializeDigitizerParameters(YAML::Emitter& stream, const DigitizerParameters& params)
	{
		stream << YAML::Key << "DigitizerParameters" << YAML::Value << YAML::BeginMap;
		stream << YAML::Key << "RecordLength" << YAML::Value << params.recordLength;
		stream << YAML::Key << "ChannelMask" << YAML::Value << params.channelMask;
		stream << YAML::Key << "EventAggr" << YAML::Value << params.eventAggr;
		stream << YAML::Key << "AcqMode" << YAML::Value << params.acqMode;
		stream << YAML::Key << "DPPAcqMode" << YAML::Value << params.dppAcqMode;
		stream << YAML::Key << "IOLevel" << YAML::Value << params.IOlevel;
		stream << YAML::Key << "TriggerMode" << YAML::Value << params.triggerMode;
		stream << YAML::Key << "SyncMode" << YAML::Value << params.syncMode;
		stream << YAML::EndMap;
	}

	static void SerializePHAChannels(YAML::Emitter& stream, const std::vector<PHAParameters>& paramList)
	{
		stream << YAML::Key << "PHAChannels" << YAML::Value << YAML::BeginSeq;
		for (auto& params : paramList)
		{
			stream << YAML::BeginMap;

			stream << YAML::Key << "IsEnabled" << YAML::Value << params.isEnabled;
			stream << YAML::Key << "PreTrigger" << YAML::Value << params.preTriggerSamples;
			stream << YAML::Key << "DCOffset" << YAML::Value << params.dcOffset;
			stream << YAML::Key << "PulsePolarity" << YAML::Value << params.pulsePolarity;
			stream << YAML::Key << "DynamicRange" << YAML::Value << params.dynamicRange;
			stream << YAML::Key << "DecayTimeConst" << YAML::Value << params.decayTimeConst;
			stream << YAML::Key << "TrapFlatTop" << YAML::Value << params.trapFlatTop;
			stream << YAML::Key << "TrapRiseTime" << YAML::Value << params.trapRiseTime;
			stream << YAML::Key << "FlatTopDelay" << YAML::Value << params.flatTopDelay;
			stream << YAML::Key << "TriggerSmoothing" << YAML::Value << params.triggerFilterSmoothing;
			stream << YAML::Key << "InputRiseTime" << YAML::Value << params.inputRiseTime;
			stream << YAML::Key << "TriggerThreshold" << YAML::Value << params.triggerThreshold;
			stream << YAML::Key << "SamplesBaseline" << YAML::Value << params.samplesBaseLineMean;
			stream << YAML::Key << "SamplesPeak" << YAML::Value << params.samplesPeakMean;
			stream << YAML::Key << "PeakHoldoff" << YAML::Value << params.peakHoldOff;
			stream << YAML::Key << "BaselineHoldoff" << YAML::Value << params.baseLineHoldOff;
			stream << YAML::Key << "TriggerHoldoff" << YAML::Value << params.triggerHoldOff;
			stream << YAML::Key << "RiseTimeWindow" << YAML::Value << params.riseTimeValidationWindow;
			stream << YAML::Key << "RiseTimeDiscrimination" << YAML::Value << params.riseTimeDiscrimination;
			stream << YAML::Key << "DigitalProbeGain" << YAML::Value << params.digitalProbeGain;
			stream << YAML::Key << "EnergyNormFactor" << YAML::Value << params.energyNormalizationFactor;
			stream << YAML::Key << "InputDecimation" << YAML::Value << params.inputDecimation;

			stream << YAML::EndMap;
		}

		stream << YAML::EndSeq;
	}

	static void SerializePSDChannels(YAML::Emitter& stream, const std::vector<PSDParameters>& paramList)
	{
		stream << YAML::Key << "PSDChannels" << YAML::Value << YAML::BeginSeq;
		for (auto& params : paramList)
		{
			stream << YAML::BeginMap;

			stream << YAML::Key << "IsEnabled" << YAML::Value << params.isEnabled;
			stream << YAML::Key << "PreTrigger" << YAML::Value << params.preTriggerSamples;
			stream << YAML::Key << "DCOffset" << YAML::Value << params.dcOffset;
			stream << YAML::Key << "PulsePolarity" << YAML::Value << params.pulsePolarity;
			stream << YAML::Key << "DynamicRange" << YAML::Value << params.dynamicRange;
			stream << YAML::Key << "BaselineThreshold" << YAML::Value << params.baselineThreshold;
			stream << YAML::Key << "TriggerHoldoff" << YAML::Value << params.triggerHoldOff;
			stream << YAML::Key << "TriggerThreshold" << YAML::Value << params.triggerThreshold;
			stream << YAML::Key << "SelfTrigger" << YAML::Value << params.selfTrigger;
			stream << YAML::Key << "ChargeSensitivity" << YAML::Value << params.chargeSensitivity;
			stream << YAML::Key << "ShortGate" << YAML::Value << params.shortGate;
			stream << YAML::Key << "LongGate" << YAML::Value << params.longGate;
			stream << YAML::Key << "PreGate" << YAML::Value << params.preGate;
			stream << YAML::Key << "TriggerWindow" << YAML::Value << params.triggerValidationWindow;
			stream << YAML::Key << "BaselineSamples" << YAML::Value << params.samplesBasline;
			stream << YAML::Key << "DiscriminatorType" << YAML::Value << params.discrminatorType;
			stream << YAML::Key << "CFDFraction" << YAML::Value << params.cfdFraction;
			stream << YAML::Key << "CFDDelay" << YAML::Value << params.cfdDelay;
			stream << YAML::Key << "TriggerConfig" << YAML::Value << params.triggerConfig;
			stream << YAML::Key << "PileUpRejection" << YAML::Value << params.pileUpRejection;
			stream << YAML::Key << "PURGap" << YAML::Value << params.purgap;

			stream << YAML::EndMap;
		}

		stream << YAML::EndSeq;
	}

	static void SerializePHAWaves(YAML::Emitter& stream, const PHAWaveParameters& waves)
	{
		stream << YAML::Key << "PHAWaveParameters" << YAML::Value << YAML::BeginMap;

		stream << YAML::Key << "IsDual" << YAML::Value << waves.isDual;
		stream << YAML::Key << "AnalogProbe1" << YAML::Value << waves.analogProbe1;
		stream << YAML::Key << "AnalogProbe2" << YAML::Value << waves.analogProbe1;
		stream << YAML::Key << "DigitalProbe1" << YAML::Value << waves.digitalProbe1;

		stream << YAML::EndMap;
	}

	static void SerializePSDWaves(YAML::Emitter& stream, const PSDWaveParameters& waves)
	{
		stream << YAML::Key << "PSDWaveParameters" << YAML::Value << YAML::BeginMap;

		stream << YAML::Key << "IsDual" << YAML::Value << waves.isDual;
		stream << YAML::Key << "AnalogProbe1" << YAML::Value << waves.analogProbe1;
		stream << YAML::Key << "AnalogProbe2" << YAML::Value << waves.analogProbe1;
		stream << YAML::Key << "DigitalProbe1" << YAML::Value << waves.digitalProbe1;
		stream << YAML::Key << "DigitalProbe2" << YAML::Value << waves.digitalProbe2;

		stream << YAML::EndMap;
	}

	//Deserialize funcs
	static DigitizerArgs DeserializeDigitizerArgs(YAML::Node& node)
	{
		DigitizerArgs args;
		args.name = node["Digitizer"].as<std::string>();
		YAML::Node& argNode = node["DigitizerArgs"];
		if (argNode)
		{
			args.type = (CAEN_DGTZ_ConnectionType) argNode.as<int>();
			args.linkNumber = argNode["LinkNumber"].as<int>();
			args.conetNode = argNode["ConetNode"].as<int>();
			args.vmeAddress = argNode["VMEAddress"].as<uint32_t>();
			args.handle = argNode["Handle"].as<int>();
			args.model = (CAEN_DGTZ_BoardModel_t) argNode["Model"].as<int>();
			args.firmware = (CAEN_DGTZ_DPPFirmware_t) argNode["Firmware"].as<int>();
			args.channels = argNode["Channels"].as<int>();
			return args;
		}
		else
		{
			BS_ERROR("Bad parse at DeserializeDigitizerArgs! No DigitizerArgs member!");
			return DigitizerArgs();
		}
	}

	
	static DigitizerParameters DeserializeDigitizerParameters(YAML::Node& node)
	{
		DigitizerParameters params;
		YAML::Node& paramNode = node["DigitizerParameters"];
		if (paramNode)
		{
			params.recordLength = paramNode["RecordLength"].as<uint32_t>();
			params.channelMask = paramNode["ChannelMask"].as<uint32_t>();
			params.eventAggr = paramNode["EventAggr"].as<int>();
			params.acqMode = (CAEN_DGTZ_AcqMode_t) paramNode["AcqMode"].as<int>();
			params.dppAcqMode = (CAEN_DGTZ_DPP_AcqMode_t) paramNode["DPPAcqMode"].as<int>();
			params.IOlevel = (CAEN_DGTZ_IOLevel_t) paramNode["IOLevel"].as<int>();
			params.triggerMode = (CAEN_DGTZ_TriggerMode_t) paramNode["TriggerMode"].as<int>();
			params.syncMode = (CAEN_DGTZ_RunSyncMode_t) paramNode["SyncMode"].as<int>();
		}
		else
		{
			BS_ERROR("Bad parse at DeserializeDigitizerParameters! No DigitizerParameters member!");
		}
		return params;
	}

	static std::vector<PHAParameters> DeserializePHAChannels(YAML::Node& node)
	{
		std::vector<PHAParameters> phaChannels;
		YAML::Node& phaNode = node["PHAChannels"];
		if (phaNode)
		{
			PHAParameters params;
			for (auto& channel : phaNode)
			{
				params.isEnabled = channel["IsEnabled"].as<bool>();
				params.preTriggerSamples = channel["PreTrigger"].as<uint32_t>();
				params.dcOffset = channel["DCOffset"].as<float>();
				params.pulsePolarity = (CAEN_DGTZ_PulsePolarity_t) channel["PulsePolarity"].as<int>();
				params.dynamicRange = (DynamicRange) channel["DynamicRange"].as<int>();
				params.decayTimeConst = channel["DecayTimeConst"].as<int>();
				params.trapFlatTop = channel["TrapFlatTop"].as<int>();
				params.trapRiseTime = channel["TrapRiseTime"].as<int>();
				params.flatTopDelay = channel["FlatTopDelay"].as<int>();
				params.triggerFilterSmoothing = channel["TriggerSmoothing"].as<int>();
				params.inputRiseTime = channel["InputRiseTime"].as<int>();
				params.triggerThreshold = channel["TriggerThreshold"].as<int>();
				params.samplesBaseLineMean = channel["SamplesBaseline"].as<int>();
				params.samplesPeakMean = channel["SamplesPeak"].as<int>();
				params.peakHoldOff = channel["PeakHoldoff"].as<int>();
				params.baseLineHoldOff = channel["BaselineHoldoff"].as<int>();
				params.triggerHoldOff = channel["TriggerHoldoff"].as<int>();
				params.riseTimeValidationWindow = channel["RiseTimeWindow"].as<int>();
				params.riseTimeDiscrimination = channel["RiseTimeDiscrimination"].as<int>();
				params.digitalProbeGain = channel["DigitalProbeGain"].as<int>();
				params.energyNormalizationFactor = channel["EnergyNormFactor"].as<int>();
				params.inputDecimation = channel["InputDecimation"].as<int>();

				phaChannels.push_back(params);
			}
		}
		else
		{
			BS_ERROR("Bad parse at DeserializePHAChannels! No PHAChannels member!");
		}

		return phaChannels;
	}

	static std::vector<PSDParameters> DeserializePSDChannels(YAML::Node& node)
	{
		std::vector<PSDParameters> psdChannels;
		YAML::Node& psdNode = node["PSDChannels"];
		if (psdNode)
		{
			PSDParameters params;
			for (auto& channel : psdNode)
			{
				params.isEnabled = psdNode["IsEnabled"].as<bool>();
				params.preTriggerSamples = psdNode["PreTrigger"].as<int>();
				params.dcOffset = psdNode["DCOffset"].as<float>();
				params.pulsePolarity = (CAEN_DGTZ_PulsePolarity_t) psdNode["PulsePolarity"].as<int>();
				params.dynamicRange = (DynamicRange) psdNode["DynamicRange"].as<int>();
				params.baselineThreshold = psdNode["BaselineThreshold"].as<int>();
				params.triggerHoldOff = psdNode["TriggerHoldoff"].as<int>();
				params.triggerThreshold = psdNode["TriggerThreshold"].as<int>();
				params.selfTrigger = psdNode["SelfTrigger"].as<int>();
				params.chargeSensitivity = psdNode["ChargeSensitivity"].as<int>();
				params.shortGate = psdNode["ShortGate"].as<int>();
				params.longGate = psdNode["LongGate"].as<int>();
				params.preGate = psdNode["PreGate"].as<int>();
				params.triggerValidationWindow = psdNode["TriggerWindow"].as<int>();
				params.samplesBasline = psdNode["BaselineSamples"].as<int>();
				params.discrminatorType = psdNode["DiscriminatorType"].as<int>();
				params.cfdFraction = psdNode["CFDFraction"].as<int>();
				params.cfdDelay = psdNode["CFDDelay"].as<int>();
				params.triggerConfig = (CAEN_DGTZ_DPP_TriggerConfig_t) psdNode["TriggerConfig"].as<int>();
				params.pileUpRejection = (CAEN_DGTZ_DPP_PUR_t) psdNode["PileUpRejection"].as<int>();
				params.purgap = psdNode["PURGap"].as<int>();

				psdChannels.push_back(params);
			}
		}
		else
		{
			BS_ERROR("Bad parse at DeserializePSDChannels! No PSDChannels member!");
		}

		return psdChannels;
	}
	*/

	ProjectSerializer::ProjectSerializer(const std::string& filepath) :
		m_filepath(filepath)
	{
	}

	ProjectSerializer::~ProjectSerializer() {}

	void ProjectSerializer::SerializeData(const BSProject::Ref& project)
	{
		std::ofstream output(m_filepath);
		if (!output.is_open())
		{
			BS_ERROR("Unable to open {0} to seralize project data!", m_filepath);
			return;
		}

		YAML::Emitter yamlStream;

		yamlStream << YAML::BeginMap;

		yamlStream << YAML::Key << "ProjectPath" << YAML::Value << project->GetProjectPath().string();
		yamlStream << YAML::Key << "RunNumber" << YAML::Value << project->GetRunNumber();
		
		std::vector<DigitizerArgs> argList = project->GetDigitizerArgsList();

		yamlStream << YAML::Key << "Digitizers" << YAML::Value << YAML::BeginSeq;
		for (auto& args : argList)
		{
			yamlStream << YAML::BeginMap;
			yamlStream << YAML::Key << "Digitizer" << YAML::Value << args.name;
			//SerializeDigitizerArgs(yamlStream, args);
			yamlStream << YAML::Key << "DigitizerArgs" << YAML::Value << args;
			yamlStream << YAML::Key << "DigitizerParameters" << YAML::Value << project->GetDigitizerParameters(args.handle);
			//SerializeDigitizerParameters(yamlStream, project->GetDigitizerParameters(args.handle));
			switch (args.firmware)
			{
				case CAEN_DGTZ_DPPFirmware_PHA:
				{
					yamlStream << YAML::Key << "PHAChannels" << YAML::Value << project->GetPHAParameters(args.handle);
					yamlStream << YAML::Key << "PHAWaves" << YAML::Value << project->GetPHAWaveParameters(args.handle);
					//SerializePHAChannels(yamlStream, project->GetPHAParameters(args.handle));
					//SerializePHAWaves(yamlStream, project->GetPHAWaveParameters(args.handle));
					break;
				}
				case CAEN_DGTZ_DPPFirmware_PSD:
				{
					yamlStream << YAML::Key << "PSDChannels" << YAML::Value << project->GetPSDParameters(args.handle);
					yamlStream << YAML::Key << "PSDWaves" << YAML::Value << project->GetPSDWaveParameters(args.handle);
					//SerializePSDChannels(yamlStream, project->GetPSDParameters(args.handle));
					//SerializePSDWaves(yamlStream, project->GetPSDWaveParameters(args.handle));
					break;
				}
				case CAEN_DGTZ_DPPFirmwareNotSupported:
				{
					break;
				}
			}
			yamlStream << YAML::EndMap;
		}
		yamlStream << YAML::EndSeq << YAML::EndMap;

		output << yamlStream.c_str();

		output.close();
	}

	void ProjectSerializer::DeserializeData(const BSProject::Ref& project)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(m_filepath);
		}
		catch (YAML::ParserException& e)
		{
			BS_ERROR("Unable to load in project settings from file {0}", m_filepath);
			return;
		}

		project->SetProjectPath(data["ProjectPath"].as<std::string>());
		project->SetRunNumber(data["RunNumber"].as<uint32_t>());

		YAML::Node& digitizers = data["Digitizers"];

		//Init some memory  for digitizer data
		DigitizerArgs args;
		DigitizerParameters params;
		std::vector<PHAParameters> phaChannels;
		std::vector<PSDParameters> psdChannels;
		PHAWaveParameters phaWaves;
		PSDWaveParameters psdWaves;

		if (digitizers)
		{
			for (auto& dgtz : digitizers)
			{
				args = dgtz["DigitizerArgs"].as<DigitizerArgs>();
				args.name = dgtz["Digitizer"].as<std::string>();
				params = dgtz["DigitizerParameters"].as<DigitizerParameters>();
				//args = DeserializeDigitizerArgs(dgtz);
				//params = DeserializeDigitizerParameters(dgtz);
				switch (args.firmware)
				{
					case CAEN_DGTZ_DPPFirmware_PHA:
					{
						phaChannels = dgtz["PHAChannels"].as<std::vector<PHAParameters>>();
						phaWaves = dgtz["PHAWaves"].as<PHAWaveParameters>();

						if (args != project->GetDigitizerArgs(args.handle))
						{
							BS_ERROR("When attempting to deserialize project settings, board mismatch detected at handle {0}! Stopping load, BoxScore should be restarted.", args.handle);
							return;
						}

						project->SetDigitizerParameters(args.handle, params);
						project->SetPHAParameters(args.handle, phaChannels);
						project->SetPHAWaveParameters(args.handle, phaWaves);
						//phaChannels = DeserializePHAChannels(dgtz);
						//phaWaves = DeserializePHAWaves(dgtz);
						break;
					}
					case CAEN_DGTZ_DPPFirmware_PSD:
					{
						psdChannels = dgtz["PSDChannels"].as<std::vector<PSDParameters>>();
						psdWaves = dgtz["PSDWaves"].as<PSDWaveParameters>();

						if (args != project->GetDigitizerArgs(args.handle))
						{
							BS_ERROR("When attempting to deserialize project settings, board mismatch detected at handle {0}! Stopping load, BoxScore should be restarted.", args.handle);
							return;
						}

						project->SetDigitizerParameters(args.handle, params);
						project->SetPSDParameters(args.handle, psdChannels);
						project->SetPSDWaveParameters(args.handle, psdWaves);
						//psdChannels = DeserializePSDChannels(dgtz);
						//psdWaves = DeserializePSDWaves(dgtz);
						break;
					}
					case CAEN_DGTZ_DPPFirmwareNotSupported:
					{
						BS_WARN("Invalid firmware detected, this better just be a debug test");

						if (args != project->GetDigitizerArgs(args.handle))
						{
							BS_ERROR("When attempting to deserialize project settings, board mismatch detected at handle {0}! Stopping load, BoxScore should be restarted.", args.handle);
							return;
						}

						project->SetDigitizerParameters(args.handle, params);
					}
				}
			}
		}
	}
}