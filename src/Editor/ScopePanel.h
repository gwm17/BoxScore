#ifndef SCOPE_PANEL_H
#define SCOPE_PANEL_H

#include "DAQ/DigitizerDefs.h"

namespace BoxScore {

	class ScopePanel
	{
	public:
		ScopePanel(int nboards);
		~ScopePanel();

		void OnUpdate();
		void OnImGuiRender();

	private:
		uint64_t m_consumerID;
		std::vector<BSData> m_buffer; //Buffered data retrieved from ring
		BSData m_selectedHit; //Hit associated with selected board/channel
		std::vector<int16_t> m_selectedXAxis; //X data is not given by DAQ, has to be made based on number of wave samples

		bool m_dataReady;
		int m_selectedBoard;
		int m_selectedChannel;
		int m_maxNumBoards;

		std::vector<std::string> m_boardListForImGui;
		std::vector<std::string> m_channelListForImGui;
	};
}

#endif