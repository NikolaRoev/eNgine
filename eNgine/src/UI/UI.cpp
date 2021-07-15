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
		bool open_node = false;
		if (entry.is_directory()) {
			open_node = ImGui::TreeNodeEx(entry.path().filename().string().c_str());
		}
		else {
			if (ImGui::Selectable(entry.path().filename().string().c_str())) {
				//TO DO: Maybe add a pop-up with the preview of the various resource files if this is clicked.
			}
		}

		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("PATH", entry.path().string().data(), entry.path().string().size());

			ImGui::Text(entry.path().filename().string().c_str());

			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (auto payload = ImGui::AcceptDragDropPayload("PATH")) {
				std::filesystem::path payload_path(std::string{ static_cast<char*>(payload->Data), static_cast<std::string::size_type>(payload->DataSize) });

				if (entry.is_directory()) {
					std::error_code error_code;
					std::filesystem::rename(payload_path, entry.path() / payload_path.filename(), error_code);
					LOG_WARN("File move operation error: {}", error_code.message());

					ImGui::EndDragDropTarget();
					if (open_node) {
						ImGui::TreePop();
					}
					break;
				}
			}

			ImGui::EndDragDropTarget();
		}

		if (open_node) {
			recursive_node_create(entry.path());

			ImGui::TreePop();
		}
	}
}

//======================================================================================================================================================

void resources_ui(ResourceManager& resource_manager, sf::RenderWindow& window) {
	ImGui::Begin("Resources");

	std::filesystem::path base_path("..\\assets");

	bool assets_node = ImGui::TreeNodeEx(base_path.filename().string().c_str(), ImGuiTreeNodeFlags_DefaultOpen);

	if (ImGui::BeginDragDropTarget()) {
		if (auto payload = ImGui::AcceptDragDropPayload("PATH")) {
			std::filesystem::path payload_path(std::string{ static_cast<char*>(payload->Data), static_cast<std::string::size_type>(payload->DataSize) });

			std::error_code error_code;
			std::filesystem::rename(payload_path, base_path / payload_path.filename(), error_code);
			LOG_WARN("File move operation error: {}", error_code.message());
		}

		ImGui::EndDragDropTarget();
	}

	if (assets_node) {
		recursive_node_create(base_path);

		ImGui::TreePop();
	}

	ImGui::End();
}

//======================================================================================================================================================
//======================================================================================================================================================