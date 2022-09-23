/*
    Digitizer.h
    Abstract base class for CAEN digitizer

    GWM -- June 2022
*/
#ifndef DIGITIZER_H
#define DIGITIZER_H

#include "DigitizerDefs.h"

namespace BoxScore {

    //Abstract digitizer base class
    class Digitizer
    {
    public:

        using Ref = std::shared_ptr<Digitizer>;

        Digitizer() :
            m_isActive(false), m_isConnected(false), m_lowBuffer(NULL), m_lowBufferSize(0), m_eventBufferSize(0),
            m_eventCountsPerChannel(nullptr), m_waveBufferSize(0)
        {
        }

        virtual ~Digitizer() { delete[] m_lowBuffer; }

        virtual void Close() = 0;

        virtual void StartAquisition() = 0;
        virtual void StopAquisition() = 0;

        virtual void SetDigitizerParameters(const DigitizerParameters& params) = 0;

        /*
            These functions look scary but really aren't. We provide a public interface that accepts any type of argument, such that the user can have a digitizer
            and call digitizer->SetChannelParamters(params)/SetWaveformParameters(params) for any valid parameter type without having to cast pointers around.
            The handling of type cases is done by the InternalSetChannel/WaveformParamters functions, which are overloaded for each case. The default behavior of 
            each setter should be to print a warning telling the user that the parameters were invalid. Then each specific derived class overrides it's specific internal
            setter and properly handles the parameters. Biggest advantage is that as new digitizer firmwares are added to the code base, the public interface basically
            doesn't change.

            This also has the advantage of implicit error handling. If the user attempts to write code for which no internal setter exists, the compiler will complain.
        */
        template<typename T>
        void SetChannelParameters(const T& params)
        {
            InternalSetChannelParameters(params);
        }

        template<typename T>
        void SetWaveformParameters(const T& params)
        {
            InternalSetWaveformParameters(params);
        }

        /*
            Similar to the setters, we have getters which allow for a simple access interface. The argument, paramBuffer, is the returned value of the state of the parameters.
        */

        template<typename T>
        void GetChannelParameters(T& paramBuffer)
        {
            InternalGetChannelParameters(paramBuffer);
        }

        template<typename T>
        void GetWaveformParameters(T& paramBuffer)
        {
            InternalGetWaveformParameters(paramBuffer);
        }

        virtual void LoadSettings() = 0;
        virtual void ReadData(std::vector<BSData>& buffer) = 0;

        const DigitizerParameters& GetDigitizerParameters() const { return m_digitizerParams; }
        const DigitizerArgs& GetDigitizerArgs() const { return m_args; }
        const bool IsActive() const { return m_isActive; }
        const bool IsConnected() const { return m_isConnected; }

    protected:
        virtual void Init(const DigitizerArgs& args, const CAEN_DGTZ_BoardInfo_t& info, int ec) = 0;

        /*
            Functions which actually set parameters. Should be an overload for each valid type. By default should always print an incompatibility warning,
            and then be overloaded by the correct derived class to actually set parameters.
        */
        virtual void InternalSetChannelParameters(const std::vector<PHAParameters>& params)
        { 
            BS_WARN("Attempting to set PHA parameters for unsupported digitizer firmware {0}", FirmwareTypeToString(m_args.firmware));
        }
        virtual void InternalSetChannelParameters(const std::vector<PSDParameters>& params)
        {
            BS_WARN("Attempting to set PSD parameters for unsupported digitizer firmware {0}", FirmwareTypeToString(m_args.firmware));
        }
        virtual void InternalSetWaveformParameters(const PHAWaveParameters& params)
        {
            BS_WARN("Attempting to set PHA wave parameters for unsupported digitizer firmware {0}", FirmwareTypeToString(m_args.firmware));
        }
        virtual void InternalSetWaveformParameters(const PSDWaveParameters& params)
        {
            BS_WARN("Attempting to set PSD wave parameters for unsupported digitizer firmware {0}", FirmwareTypeToString(m_args.firmware));
        }

        virtual void InternalGetChannelParameters(std::vector<PHAParameters>& paramBuffer)
        {
            BS_WARN("Attempting to get PHA parameters for unsupported digitizer firmware {0}", FirmwareTypeToString(m_args.firmware));
        }
        virtual void InternalGetChannelParameters(std::vector<PSDParameters>& paramBuffer)
        {
            BS_WARN("Attempting to get PSD parameters for unsupported digitizer firmware {0}", FirmwareTypeToString(m_args.firmware));
        }
        virtual void InternalGetWaveformParameters(PHAWaveParameters& paramBuffer)
        {
            BS_WARN("Attempting to get PHA wave parameters for unsupported digitizer firmware {0}", FirmwareTypeToString(m_args.firmware));
        }
        virtual void InternalGetWaveformParameters(PSDWaveParameters& paramBuffer)
        {
            BS_WARN("Attempting to get PSD wave parameters for unsupported digitizer firmware {0}", FirmwareTypeToString(m_args.firmware));
        }

        DigitizerArgs m_args;
        DigitizerParameters m_digitizerParams;

        bool m_isActive;
        bool m_isConnected;

        char* m_lowBuffer;
        uint32_t m_lowBufferSize;

        uint32_t m_eventBufferSize;
        uint32_t* m_eventCountsPerChannel;
        uint32_t m_waveBufferSize;

        CAEN_DGTZ_BoardInfo_t m_internalData; //internal use only
    };

    //Creation function
    Digitizer::Ref OpenDigitizer(DigitizerArgs& args);

    //Digitizers are better sliced by Firmware than by model. A lot of boards support multpile firmwares.

    class DigitizerPHA : public Digitizer
    {
    public:
        DigitizerPHA(const DigitizerArgs& args, const CAEN_DGTZ_BoardInfo_t& info, int ec);
        virtual ~DigitizerPHA();

        virtual void Close() override;

        virtual void StartAquisition() override;
        virtual void StopAquisition() override;

        virtual void LoadSettings() override;

        virtual void SetDigitizerParameters(const DigitizerParameters& params) override;
        virtual void ReadData(std::vector<BSData>& buffer) override;

    private:
        virtual void Init(const DigitizerArgs& args, const CAEN_DGTZ_BoardInfo_t& info, int ec) override;

        virtual void InternalSetChannelParameters(const std::vector<PHAParameters>& params) override;
        virtual void InternalSetWaveformParameters(const PHAWaveParameters& params) override;
        virtual void InternalGetChannelParameters(std::vector<PHAParameters>& paramBuffer) override { paramBuffer = m_channelParams; }
        virtual void InternalGetWaveformParameters(PHAWaveParameters& paramBuffer) override { paramBuffer = m_waveParams; }

        void LoadDigitizerParameters();
        void LoadChannelParameters();
        void LoadWaveformParameters();
        void AllocateMemory();
        void DeallocateMemory();

        //CAEN required data storage, does not interface to other parts of the program
        CAEN_DGTZ_DPP_PHA_Event_t** m_eventData;
        CAEN_DGTZ_DPP_PHA_Waveforms_t** m_waveData;
        CAEN_DGTZ_DPP_PHA_Params_t m_caenParams;

        std::vector<PHAParameters> m_channelParams;
        PHAWaveParameters m_waveParams;
    };

    class DigitizerPSD : public Digitizer
    {
    public:
        DigitizerPSD(const DigitizerArgs& args, const CAEN_DGTZ_BoardInfo_t& info, int ec);
        virtual ~DigitizerPSD();

        virtual void Close() override;

        virtual void StartAquisition() override;
        virtual void StopAquisition() override;

        virtual void LoadSettings() override;

        virtual void SetDigitizerParameters(const DigitizerParameters& params) override;
        virtual void ReadData(std::vector<BSData>& buffer) override;

    private:
        virtual void Init(const DigitizerArgs& args, const CAEN_DGTZ_BoardInfo_t& info, int ec) override;

        virtual void InternalSetChannelParameters(const std::vector<PSDParameters>& params) override;
        virtual void InternalSetWaveformParameters(const PSDWaveParameters& params) override;
        virtual void InternalGetChannelParameters(std::vector<PSDParameters>& paramBuffer) override { paramBuffer = m_channelParams; }
        virtual void InternalGetWaveformParameters(PSDWaveParameters& paramBuffer) override { paramBuffer = m_waveParams; }

        void LoadDigitizerParameters();
        void LoadChannelParameters();
        void LoadWaveformParameters();
        void AllocateMemory();
        void DeallocateMemory();

        //CAEN required data storage, does not interface to other parts of the program
        CAEN_DGTZ_DPP_PSD_Event_t** m_eventData;
        CAEN_DGTZ_DPP_PSD_Waveforms_t** m_waveData;
        CAEN_DGTZ_DPP_PSD_Params_t m_caenParams;

        std::vector<PSDParameters> m_channelParams;
        PSDWaveParameters m_waveParams;
    };

}


#endif