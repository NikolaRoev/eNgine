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
	//Hold value for a path selected for deletion.
	std::filesystem::path path_to_delete{};

	for (auto& entry : std::filesystem::directory_iterator(path)) {
		bool open_node = false;

		//Handle folders.
		if (entry.is_directory()) {
			open_node = ImGui::TreeNodeEx(entry.path().filename().string().c_str());

			//Right click context menu.
			if (ImGui::BeginPopupContextItem()) {
				ImGui::Text("Add");
				ImGui::Separator();

				if (ImGui::Button("Existing File")) {
					if (auto add_path = open_file(window); add_path.has_value()) {
						std::error_code error_code;
						std::filesystem::copy_file(add_path.value(), entry.path() / add_path.value().filename(), error_code);

						if (error_code) {
							LOG_WARN("File copy operation error: {}", error_code.message());
						}
					}

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("New Folder")) {
					std::error_code error_code;

					//Handle if there already is a "New Folder \ (n)".
					if (!std::filesystem::exists(entry.path() / "New Folder", error_code)) {
						std::filesystem::create_directory(entry.path() / "New Folder", error_code);

						if (error_code) {
							LOG_WARN("Create \"New Folder\" error: {}", error_code.message());
						}
					}
					else {
						size_t new_folder_number = 1;
						while (std::filesystem::exists(entry.path() / std::format("New Folder ({})", new_folder_number), error_code)) {
							new_folder_number++;
						}
						std::filesystem::create_directory(entry.path() / std::format("New Folder ({})", new_folder_number), error_code);

						if (error_code) {
							LOG_WARN("Create \"New Folder\" error: {}", error_code.message());
						}
					}

					ImGui::CloseCurrentPopup();
				}

				static std::string temp_name;
				ImGui::InputText("Rename", &temp_name);

				if (ImGui::Button("Rename##Button")) {
					std::filesystem::path temp_new_path(entry.path().parent_path());
					temp_new_path /= temp_name;

					std::error_code error_code;
					std::filesystem::rename(entry.path(), temp_new_path, error_code);

					if (error_code) {
						LOG_WARN("Rename error: {}", error_code.message());
					}

					temp_name = "";
					ImGui::CloseCurrentPopup();

				}

				if (ImGui::Button("Delete")) {
					path_to_delete = entry.path();

					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		//Other files.
		else {
			if (ImGui::Selectable(entry.path().filename().string().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					run_file(entry.path().string());
				}
			}
			//Right click context menu.
			if (ImGui::BeginPopupContextItem()) {

				static std::string temp_name;
				ImGui::InputText("Rename", &temp_name);

				if (ImGui::Button("Rename##Button")) {
					std::filesystem::path temp_new_path(entry.path().parent_path());
					temp_new_path /= temp_name;
					temp_new_path += entry.path().extension();

					std::error_code error_code;
					std::filesystem::rename(entry.path(), temp_new_path, error_code);

					if (error_code) {
						LOG_WARN("Rename error: {}", error_code.message());
					}

					temp_name = "";
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("Delete")) {
					path_to_delete = entry.path();

					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		
		//Drag drop source and target for either the TreeNode or Selectable.
		//Gets the path for the file or the folder.
		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("PATH", entry.path().string().data(), entry.path().string().size());

			ImGui::Text(entry.path().filename().string().c_str());

			ImGui::EndDragDropSource();
		}

		//If target is folder moves the Payload to the folder, if target is file, moves the Payload to the folder that file is in.
		if (ImGui::BeginDragDropTarget()) {
			if (auto payload = ImGui::AcceptDragDropPayload("PATH")) {
				std::filesystem::path payload_path(std::string{ static_cast<char*>(payload->Data), static_cast<std::string::size_type>(payload->DataSize) });

				if (entry.is_directory()) {
					std::error_code error_code;
					std::filesystem::rename(payload_path, entry.path() / payload_path.filename(), error_code);

					if (error_code) {
						LOG_WARN("File move operation error: {}", error_code.message());
					}

					ImGui::EndDragDropTarget();
					if (open_node) {
						ImGui::TreePop();
					}
					break;
				}
				else {
					std::error_code error_code;
					std::filesystem::rename(payload_path, entry.path().parent_path() / payload_path.filename(), error_code);

					if (error_code) {
						LOG_WARN("File move operation error: {}", error_code.message());
					}

					ImGui::EndDragDropTarget();
					if (open_node) {
						ImGui::TreePop();
					}
					break;
				}
			}

			ImGui::EndDragDropTarget();
		}

		//Recursive call and TreePop here so we can generate DragDropSource and Target even if TreeNode is not open.
		if (open_node) {
			recursive_node_create(entry.path(), window);
			ImGui::TreePop();
		}
	}

	//Handle if a path has been selected for deletion.
	if (!path_to_delete.empty()) {
		std::error_code error_code;
		std::filesystem::remove_all(path_to_delete, error_code);

		if (error_code) {
			LOG_WARN("Delete error: {}", error_code.message());
		}
	}
}

//======================================================================================================================================================

void resources_ui(ResourceManager& resource_manager, sf::RenderWindow& window) {
	//Base path where all assets are put in. Can't rename/move/delete this folder.
	const std::filesystem::path base_path("assets");


	ImGui::Begin("Resources");

	bool assets_node = ImGui::TreeNodeEx(base_path.filename().string().c_str(), ImGuiTreeNodeFlags_DefaultOpen);

	//Right click context menu.
	if (ImGui::BeginPopupContextItem()) {
		ImGui::Text("Add");
		ImGui::Separator();

		if (ImGui::Button("Existing File")) {
			if (auto add_path = open_file(window); add_path.has_value()) {
				std::error_code error_code;
				std::filesystem::copy_file(add_path.value(), base_path / add_path.value().filename(), error_code);

				if (error_code) {
					LOG_WARN("File copy operation error: {}", error_code.message());
				}
			}

			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("New Folder")) {
			std::error_code error_code;

			//Handle if there already is a "New Folder \ (n)".
			if (!std::filesystem::exists(base_path / "New Folder", error_code)) {
				std::filesystem::create_directory(base_path / "New Folder", error_code);

				if (error_code) {
					LOG_WARN("Create \"New Folder\" error: {}", error_code.message());
				}
			}
			else {
				size_t new_folder_number = 1;
				while (std::filesystem::exists(base_path / std::format("New Folder ({})", new_folder_number), error_code)) {
					new_folder_number++;
				}
				std::filesystem::create_directory(base_path / std::format("New Folder ({})", new_folder_number), error_code);

				if (error_code) {
					LOG_WARN("Create \"New Folder\" error: {}", error_code.message());
				}
			}

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	//We only need the "assets" TreeNode as a DragDropTarget as we are not going to move it.
	if (ImGui::BeginDragDropTarget()) {
		if (auto payload = ImGui::AcceptDragDropPayload("PATH")) {
			std::filesystem::path payload_path(std::string{ static_cast<char*>(payload->Data), static_cast<std::string::size_type>(payload->DataSize) });

			std::error_code error_code;
			std::filesystem::rename(payload_path, base_path / payload_path.filename(), error_code);

			if (error_code) {
				LOG_WARN("File move operation error: {}", error_code.message());
			}
		}

		ImGui::EndDragDropTarget();
	}

	//Recursive call and TreePop here so we can generate DragDropSource and Target even if TreeNode is not open.
	if (assets_node) {
		recursive_node_create(base_path, window);

		ImGui::TreePop();
	}

	ImGui::End();
}

//======================================================================================================================================================
//======================================================================================================================================================