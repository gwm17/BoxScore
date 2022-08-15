#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <filesystem>
#include "imgui.h"

namespace BoxScore {

	class FileDialog
	{
	public:

		enum class Type
		{
			OpenFile,
			SaveFile,
			OpenDir,
			SaveDir,
			None
		};

		FileDialog();
		~FileDialog();

		inline void OpenDialog(Type type) { m_type = type; m_openDialogFlag = true; }
		std::pair<std::string, Type> RenderFileDialog(const std::string& ext = "");

	private:
		std::string ImGuiRenderOpenFile(const std::string& ext);
		std::string ImGuiRenderSaveFile(const std::string& ext);
		std::string ImGuiRenderOpenDir();

		std::filesystem::path m_currentPath;
		Type m_type;

		std::string m_selectedItem;

		bool m_openDialogFlag;

		ImGuiTableFlags table_flags;
		ImGuiSelectableFlags select_flags;
	};

}

#endif