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
            m_isActive(false), m_isConnected(false), m_lowBuffer(NULL) //CAEN requires this to be NULL
        {
        }

        virtual ~Digitizer() { delete[] m_lowBuffer; }

        virtual void Close() = 0;

        virtual void StartAquisition() = 0;
        virtual void StopAquisition() = 0;

        virtual void SetDigitizerParameters(const DigitizerParameters& params) = 0;
        virtual void LoadSettings() = 0;
        virtual void ReadData(std::vector<BSData>& buffer) = 0;

        const DigitizerParameters& GetDigitizerParameters() const { return m_digitizerParams; }
        const DigitizerArgs& GetDigitizerArgs() const { return m_args; }
        const bool IsActive() const { return m_isActive; }
        const bool IsConnected() const { return m_isConnected; }

    protected:
        virtual void Init(const DigitizerArgs& args, const CAEN_DGTZ_BoardInfo_t& info, int ec) = 0;

        DigitizerArgs m_args;
        DigitizerParameters m_digitizerParams;

        bool m_isActive;
        bool m_isConnected;

        char* m_lowBuffer;
        uint32_t m_lowBufferSize;

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
        void SetChannelParameters(const std::vector<PHAParameters>& params);
        void SetWaveformParameters(const PHAWaveParameters& params);
        virtual void ReadData(std::vector<BSData>& buffer) override;

        const std::vector<PHAParameters>& GetChannelParameters() { return m_channelParams; }
        const PHAWaveParameters& GetWaveformParameters() { return m_waveParams; }

    private:
        virtual void Init(const DigitizerArgs& args, const CAEN_DGTZ_BoardInfo_t& info, int ec) override;
        void LoadDigitizerParameters();
        void LoadChannelParameters();
        void LoadWaveformParameters();
        void AllocateMemory();
        void DeallocateMemory();

        CAEN_DGTZ_DPP_PHA_Event_t** m_eventData; //middle man CAEN data types, required raw
        uint32_t m_eventBufferSize;
        uint32_t* m_eventCountsPerChannel;
        CAEN_DGTZ_DPP_PHA_Waveforms_t** m_waveData;
        uint32_t m_waveBufferSize;

        std::vector<PHAParameters> m_channelParams;
        PHAWaveParameters m_waveParams;
        CAEN_DGTZ_DPP_PHA_Params_t m_caenParams;
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
        void SetChannelParameters(const std::vector<PSDParameters>& params);
        void SetWaveformParameters(const PSDWaveParameters& params);
        virtual void ReadData(std::vector<BSData>& buffer) override;

        const std::vector<PSDParameters>& GetChannelParameters() { return m_channelParams; }
        const PSDWaveParameters& GetWaveformParameters() { return m_waveParams; }

    private:
        virtual void Init(const DigitizerArgs& args, const CAEN_DGTZ_BoardInfo_t& info, int ec) override;
        void LoadDigitizerParameters();
        void LoadChannelParameters();
        void LoadWaveformParameters();
        void AllocateMemory();
        void DeallocateMemory();

        CAEN_DGTZ_DPP_PSD_Event_t** m_eventData; //middle man CAEN data types
        uint32_t m_eventBufferSize;
        uint32_t* m_eventCountsPerChannel;
        CAEN_DGTZ_DPP_PSD_Waveforms_t** m_waveData;
        uint32_t m_waveBufferSize;

        std::vector<PSDParameters> m_channelParams;
        PSDWaveParameters m_waveParams;
        CAEN_DGTZ_DPP_PSD_Params_t m_caenParams;
    };

}


#endif