#include "pch.h"
#include "UI.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Utility.h"

//======================================================================================================================================================
//======================================================================================================================================================

void menu_bar_ui() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

//======================================================================================================================================================
//======================================================================================================================================================

void recursive_node_create(const std::filesystem::path& path) {
	for (auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_directory()) {
			if (ImGui::TreeNode(entry.path().filename().string().c_str())) {
				recursive_node_create(entry.path());

				ImGui::TreePop();
			}
		}
		else {
			if (ImGui::Selectable(entry.path().filename().string().c_str())) {
				//TO DO: Maybe add a pop-up with the preview of the various resource files if this is clicked.
			}
		}

			
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
			ImGui::SetDragDropPayload("PATH", entry.path().string().data(), entry.path().string().size());

			ImGui::Text(entry.path().filename().string().c_str());

			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (auto payload = ImGui::AcceptDragDropPayload("PATH")) {
				std::filesystem::path payload_path(std::string{ static_cast<char*>(payload->Data), static_cast<std::string::size_type>(payload->DataSize) });

				//TO DO: Handle if dragdrop source and dragdrop target are either file or directory. Good luck lul.
			}

			ImGui::EndDragDropTarget();
		}
	}
}

//======================================================================================================================================================

void resources_ui(ResourceManager& resource_manager, sf::RenderWindow& window) {
	ImGui::Begin("Resources");

	recursive_node_create("../assets");

	ImGui::End();
}

//======================================================================================================================================================
//======================================================================================================================================================