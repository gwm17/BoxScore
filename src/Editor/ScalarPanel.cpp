#include "ScalarPanel.h"

#include "imgui.h"

namespace BoxScore {

	ScalarPanel::ScalarPanel() :
		m_elapsedTime(0.0)
	{
	}

	ScalarPanel::~ScalarPanel()
	{
	}

	void ScalarPanel::OnImGuiRender(const std::vector<DigitizerArgs>& argList, double timestep)
	{
		static bool changedBoard = false;
		m_elapsedTime += timestep;
		if (m_elapsedTime >= 60.0f) //Update the rates once a minute
		{
			ScalarDistributor::CalculateRates(m_elapsedTime);
			m_elapsedTime = 0.0f; //Reset it
		}

		if (ImGui::Begin("ScalarPanel"))
		{
			if (ImGui::BeginCombo("Selected Board", m_selectedArgs.name.c_str()))
			{
				for (auto& args : argList)
				{
					if (ImGui::Selectable(args.name.c_str(), m_selectedArgs.name == args.name))
					{
						m_selectedArgs = args;
						changedBoard = true;
					}
				}

				ImGui::EndCombo();
			}

			if (changedBoard)
			{
				m_scalarNames.resize(m_selectedArgs.channels);
				for (int i = 0; i < m_selectedArgs.channels; i++)
					m_scalarNames[i] = m_selectedArgs.name + std::to_string(i);

				changedBoard = false;
			}

			if (ImGui::BeginTable("ScalarTable", 2))
			{
				ImGui::TableSetupColumn("Channel Number");
				ImGui::TableSetupColumn("Throughput Rate");
				ImGui::TableHeadersRow();

				for (int i = 0; i < m_selectedArgs.channels; i++)
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%d", i);
					ImGui::TableNextColumn();
					ImGui::Text("%f", ScalarDistributor::GetRate(m_scalarNames[i]));
				}

				ImGui::EndTable();
			}
		}
		ImGui::End();
	}
}