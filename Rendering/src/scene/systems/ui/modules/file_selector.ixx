module;

#include <functional>

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.cpp"

#include "io/io.h"

export module rendering:systems.user_interface.modules.file_selector;


export class FileSelector final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	FileSelector(fs::path root_dir = fs::current_path()) : open(false) {
		if (fs::exists(root_dir)) {
			setPath(root_dir);
		}
		else {
			setPath(fs::current_path());
		}
	}

	FileSelector(const FileSelector&) = default;
	FileSelector(FileSelector&&) = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~FileSelector() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	FileSelector& operator=(const FileSelector&) = default;
	FileSelector& operator=(FileSelector&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	void openSelector() {
		open = true;
	}

	[[nodiscard]]
	bool update() {
		bool file_selected = false;

		if (open) {
			ImGui::OpenPopup("FileSelector");
			open = false;
		}

		if (ImGui::BeginPopupModal("FileSelector", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

			//----------------------------------------------------------------------------------
			// Nav Bar
			//----------------------------------------------------------------------------------
			if (ImGui::BeginChild("NavBar", {425, 40}, ImGuiChildFlags_Border, ImGuiWindowFlags_NoSavedSettings)) {
				if (ImGui::Button("^")) {
					setPath(current_path / fs::path{".."});
				}

				ImGui::SameLine();

				std::string path_string = current_path.string();
				ImGui::PushItemWidth(350);
				ImGui::InputText("", &nav_bar_text);
				ImGui::PopItemWidth();

				ImGui::SameLine();

				if (ImGui::Button("Go")) {
					if (fs::exists(nav_bar_text))
						setPath(nav_bar_text);
				}
			}
			ImGui::EndChild();


			//----------------------------------------------------------------------------------
			// File List
			//----------------------------------------------------------------------------------
			if (ImGui::BeginChild("FileList", {425, 300}, ImGuiChildFlags_Border, ImGuiWindowFlags_NoSavedSettings)) {
				listFiles(fs::directory_iterator{current_path});
			}
			ImGui::EndChild();

			if (ImGui::BeginChild("FileBar", {425, 40}, ImGuiChildFlags_Border, ImGuiWindowFlags_NoSavedSettings)) {
				ImGui::Text("File:");
				ImGui::SameLine();

				// Filename Box
				std::string selected_string = selected_file.string();
				ImGui::PushItemWidth(250);
				ImGui::InputText("", &selected_string, ImGuiInputTextFlags_ReadOnly);
				ImGui::PopItemWidth();
			
				ImGui::SameLine();

				// OK/Cancel Buttons
				ImGui::BeginGroup();
				if (ImGui::Button("Open")) {
					if (fs::exists(selected_file))
						file_selected = true;
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel")) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndGroup();
			}
			ImGui::EndChild();

			ImGui::EndPopup();
		}

		return file_selected;
	}

	[[nodiscard]]
	fs::path getSelectedFile() {
		return selected_file;
	}

private:

	void setPath(const fs::path& path) {
		current_path = fs::absolute(path);
		nav_bar_text = current_path.string();
	}

	void listFiles(std::filesystem::directory_iterator it) {
		for (auto& p : it) {
			const auto p_path = p.path();
			if (p.is_directory()) {  //Directory
				static const auto flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_OpenOnDoubleClick;

				ImGui::PushStyleColor(ImGuiCol_Text, {1.0f, 0.88f, 0.0f, 1.0f});
				if (ImGui::TreeNodeEx(p_path.filename().string().c_str(), flags)) {
					if (ImGui::IsItemActive())
						setPath(p_path);
					ImGui::TreePop();
				}
				ImGui::PopStyleColor();
			}
			else {  //File
				std::error_code err; //err passed to fs::equivalent to suppress potential run-time exception
				const bool selected = fs::equivalent(p_path, selected_file, err);
				const auto flags = ImGuiTreeNodeFlags_Leaf | (selected ? ImGuiTreeNodeFlags_Selected : 0);

				if (ImGui::TreeNodeEx(p_path.filename().string().c_str(), flags)) {
					if (ImGui::IsItemActive())
						selected_file = p_path;
					ImGui::TreePop();
				}
			}
		}
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	bool open;
	std::string nav_bar_text;
	fs::path current_path;
	fs::path selected_file;
};