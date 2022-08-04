#include "SyncDialog.h"

#include "imgui.h"

namespace BoxScore {

	static std::string SyncMethodToString(SyncMethod method)
	{
		switch (method)
		{
		case SyncMethod::SIn_TrigOut: return "SyncIn-TrigOut";
		case SyncMethod::None: return "None";
		}
		return "None";
	}

	static std::string StartTypeToString(SyncStartType type)
	{
		switch (type)
		{
		case SyncStartType::HardwareControlled: return "Hardware-Controlled";
		case SyncStartType::SoftwareControlled: return "Software-Controlled";
		case SyncStartType::None: return "None";
		}
		return "None";
	}

	SyncDialog::SyncDialog() :
		m_open(false)
	{
	}

	SyncDialog::~SyncDialog() {}

	bool SyncDialog::OnImGuiRender()
	{
		bool status = false;
		if (m_open)
		{
			ImGui::OpenPopup("SyncDialog");
			status = false;
			m_open = false;
		}

		if (ImGui::BeginPopupModal("SyncDialog"))
		{
			if (ImGui::BeginCombo("Method", SyncMethodToString(m_syncArgs.syncMethod).c_str()))
			{
				if (ImGui::Selectable("SyncIn-TrigOut", SyncMethod::SIn_TrigOut == m_syncArgs.syncMethod, ImGuiSelectableFlags_DontClosePopups))
					m_syncArgs.syncMethod = SyncMethod::SIn_TrigOut;
				if (ImGui::Selectable("None", SyncMethod::None == m_syncArgs.syncMethod, ImGuiSelectableFlags_DontClosePopups))
					m_syncArgs.syncMethod = SyncMethod::None;
			}
			if (ImGui::BeginCombo("Start type", StartTypeToString(m_syncArgs.startType).c_str()))
			{
				if (ImGui::Selectable("Hardware-Controlled", SyncStartType::HardwareControlled == m_syncArgs.startType, ImGuiSelectableFlags_DontClosePopups))
					m_syncArgs.startType = SyncStartType::HardwareControlled;
				if (ImGui::Selectable("Software-Controlled", SyncStartType::SoftwareControlled == m_syncArgs.startType, ImGuiSelectableFlags_DontClosePopups))
					m_syncArgs.startType = SyncStartType::SoftwareControlled;
			}

			if (ImGui::Button("Ok"))
			{
				status = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				status = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		return status;
	}
}