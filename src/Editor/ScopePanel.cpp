#include "ScopePanel.h"
#include "Core/RingBuffer.h"

#include "imgui.h"
#include "implot.h"

namespace BoxScore {

	ScopePanel::ScopePanel(int nboards) :
		m_dataReady(false), m_selectedBoard(0), m_selectedChannel(0), m_maxNumBoards(nboards)
	{
		m_consumerID = RingBuffer::Attach();
		for (int i = 0; i < nboards; i++)
			m_boardListForImGui.push_back(fmt::format("{0}", i));
		for (int i = 0; i < 16; i++) //bad hardcode, fix later
			m_channelListForImGui.push_back(fmt::format("{0}", i));
	}

	ScopePanel::~ScopePanel()
	{
		RingBuffer::Detach(m_consumerID);
	}

	void ScopePanel::OnUpdate()
	{
		static bool bufferFilled = false;
		bufferFilled = RingBuffer::PopData(m_consumerID, m_buffer);
		if (bufferFilled)
		{
			for (auto& hit : m_buffer)
			{
				if (hit.board == m_selectedBoard && hit.channel == m_selectedChannel)
				{
					m_selectedHit = hit;
					m_selectedXAxis.clear();
					for (int i = 0; i < hit.waveSize; i++)
						m_selectedXAxis.push_back(i);
					break;
				}
			}
		}
	}

	void ScopePanel::OnImGuiRender()
	{
		static std::string selectedBoardString = fmt::format("{0}", m_selectedBoard);
		static std::string selectedChannelString = fmt::format("{0}", m_selectedChannel);

		if (ImGui::Begin("Oscilloscope"))
		{
			if (ImGui::BeginCombo("Board", selectedBoardString.c_str()))
			{
				for (int board=0; board<m_maxNumBoards; board++)
				{
					if (ImGui::Selectable(m_boardListForImGui[board].c_str(), board == m_selectedBoard))
					{
						m_selectedBoard = board;
						selectedBoardString = fmt::format("{0}", m_selectedBoard);
						m_selectedXAxis.clear();
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::BeginCombo("Channel", selectedChannelString.c_str()))
			{
				for (int channel = 0; channel < 16; channel++) //hardcoded bad, fix later
				{
					if (ImGui::Selectable(m_channelListForImGui[channel].c_str(), channel == m_selectedChannel))
					{
						m_selectedChannel = channel;
						selectedChannelString = fmt::format("{0}", m_selectedChannel);
						m_selectedXAxis.clear();
					}
				}
				ImGui::EndCombo();
			}
			if (ImPlot::BeginPlot("ScopeView", ImVec2(-1,-1)))
			{
				if (m_selectedXAxis.size() != 0)
				{
					ImPlot::PlotLine("AnalogProbe1", (ImU16*)m_selectedXAxis.data(), (ImU16*)m_selectedHit.trace1Samples.data(), m_selectedXAxis.size());
					ImPlot::PlotLine("AnalogProbe2", (ImU16*)m_selectedXAxis.data(), (ImU16*)m_selectedHit.trace2Samples.data(), m_selectedXAxis.size());
					ImPlot::PlotLine("DigitialProbe1", (ImU16*)m_selectedXAxis.data(), (ImU16*)m_selectedHit.digitalTrace1Samples.data(), m_selectedXAxis.size());
					ImPlot::PlotLine("DigitialProbe2", (ImU16*)m_selectedXAxis.data(), (ImU16*)m_selectedHit.digitalTrace2Samples.data(), m_selectedXAxis.size());
				}
				ImPlot::EndPlot();
			}
		}
		ImGui::End();
	}
}