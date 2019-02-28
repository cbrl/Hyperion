#include "file_selector.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.cpp"
#include <functional>

FileSelector::FileSelector(fs::path root_dir)
	: open(false) {
	if (fs::exists(root_dir)) {
		setPath(root_dir);
	}
	else {
		setPath(fs::current_path());
	}
}


void FileSelector::openSelector() {
	open = true;
}


fs::path FileSelector::getSelectedFile() {
	return selected_file;
}


void FileSelector::setPath(const fs::path& path) {
	current_path = fs::absolute(path);
	nav_bar_text = current_path.string();
}


bool FileSelector::update() {
	bool file_selected = false;

	if (open) {
		ImGui::OpenPopup("FileSelector");
		open = false;
	}

	if (ImGui::BeginPopupModal("FileSelector", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

		//----------------------------------------------------------------------------------
		// Nav Bar
		//----------------------------------------------------------------------------------
		if (ImGui::BeginChild("NavBar", {425, 40}, true, ImGuiWindowFlags_NoSavedSettings)) {
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
		if (ImGui::BeginChild("FileList", {425, 300}, true, ImGuiWindowFlags_NoSavedSettings)) {
			listFiles(fs::directory_iterator{current_path});
		}
		ImGui::EndChild();

		if (ImGui::BeginChild("FileBar", {425, 40}, true, ImGuiWindowFlags_NoSavedSettings)) {
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


void FileSelector::listFiles(std::filesystem::directory_iterator it) {
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
