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

void recursive_node_create(const std::filesystem::path& path, sf::RenderWindow& window) {
	std::filesystem::path path_to_delete{};

	for (auto& entry : std::filesystem::directory_iterator(path)) {
		bool open_node = false;
		if (entry.is_directory()) {
			open_node = ImGui::TreeNodeEx(entry.path().filename().string().c_str());

			if (ImGui::BeginPopupContextItem()) {
				ImGui::Text("Add");
				ImGui::Separator();

				if (ImGui::Button("Existing File")) {
					ImGui::CloseCurrentPopup();

					if (auto add_path = open_file(window); add_path.has_value()) {
						std::error_code error_code;
						std::filesystem::copy_file(add_path.value(), entry.path() / add_path.value().filename(), error_code);
						LOG_WARN("File copy operation error: {}", error_code.message());
					}
				}

				if (ImGui::Button("New Folder")) {
					ImGui::CloseCurrentPopup();

					std::error_code error_code;
					std::filesystem::create_directory(entry.path() / "New Folder", error_code);
					LOG_WARN("Create \"New Folder\" error: {}", error_code.message());
				}

				static std::string temp_name;
				ImGui::InputText("Rename", &temp_name);

				if (ImGui::Button("Rename##Button")) {
					std::filesystem::path temp_new_path(entry.path().parent_path());
					temp_new_path /= temp_name;

					std::error_code error_code;
					std::filesystem::rename(entry.path(), temp_new_path, error_code);
					LOG_WARN("Rename error: {}", error_code.message());

					ImGui::CloseCurrentPopup();
					temp_name = "";

				}

				if (ImGui::Button("Delete")) {
					path_to_delete = entry.path();

					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		else {
			if (ImGui::Selectable(entry.path().filename().string().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					run_file(entry.path().string());
				}
			}
			if (ImGui::BeginPopupContextItem()) {

				static std::string temp_name;
				ImGui::InputText("Rename", &temp_name);

				if (ImGui::Button("Rename##Button")) {
					std::filesystem::path temp_new_path(entry.path().parent_path());
					temp_new_path /= temp_name;
					temp_new_path += entry.path().extension();

					std::error_code error_code;
					std::filesystem::rename(entry.path(), temp_new_path, error_code);
					LOG_WARN("Rename error: {}", error_code.message());

					ImGui::CloseCurrentPopup();
					temp_name = "";

				}

				if (ImGui::Button("Delete")) {
					path_to_delete = entry.path();

					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
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
				else {
					std::error_code error_code;
					std::filesystem::rename(payload_path, entry.path().parent_path() / payload_path.filename(), error_code);
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
			recursive_node_create(entry.path(), window);
			ImGui::TreePop();
		}
	}

	if (!path_to_delete.empty()) {
		std::error_code error_code;
		std::filesystem::remove_all(path_to_delete, error_code);
		LOG_WARN("Delete error: {}", error_code.message());
	}
}

//======================================================================================================================================================

void resources_ui(ResourceManager& resource_manager, sf::RenderWindow& window) {
	ImGui::Begin("Resources");

	std::filesystem::path path_to_delete{};

	std::filesystem::path base_path("..\\assets");
	bool assets_node = ImGui::TreeNodeEx(base_path.filename().string().c_str(), ImGuiTreeNodeFlags_DefaultOpen);

	if (ImGui::BeginPopupContextItem()) {
		ImGui::Text("Add");
		ImGui::Separator();

		if (ImGui::Button("Existing File")) {
			ImGui::CloseCurrentPopup();

			if (auto add_path = open_file(window); add_path.has_value()) {
				std::error_code error_code;
				std::filesystem::copy_file(add_path.value(), base_path / add_path.value().filename(), error_code);
				LOG_WARN("File copy operation error: {}", error_code.message());
			}
		}

		if (ImGui::Button("New Folder")) {
			ImGui::CloseCurrentPopup();

			std::error_code error_code;
			std::filesystem::create_directory(base_path / "New Folder", error_code);
			LOG_WARN("Create \"New Folder\" error: {}", error_code.message());
		}

		ImGui::EndPopup();
	}

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
		recursive_node_create(base_path, window);

		ImGui::TreePop();
	}

	ImGui::End();
}

//======================================================================================================================================================
//======================================================================================================================================================