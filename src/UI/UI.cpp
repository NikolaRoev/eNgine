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
			open_node = ImGui::TreeNodeEx(std::format("{}##{}", entry.path().filename().string(), entry.path().string()).c_str());

			//Right click context menu.
			if (ImGui::BeginPopupContextItem()) {
				//"Add" sub-menu.
				if (ImGui::BeginMenu("Add##Recursive")) {
					//Existing resource file button.
					if (ImGui::MenuItem("Resource File##Recursive")) {
						if (auto add_path = open_file(window); add_path.has_value()) {
							std::error_code error_code;
							std::filesystem::copy_file(add_path.value(), entry.path() / add_path.value().filename(), error_code);

							if (error_code) {
								LOG_WARN("File copy operation error: {}", error_code.message());
							}
						}
					}
					//New folder button.
					if (ImGui::MenuItem("New Folder##Recursive")) {
						std::error_code error_code;

						//Create the new folder.
						if (!std::filesystem::exists(entry.path() / "New Folder", error_code)) {
							std::filesystem::create_directory(entry.path() / "New Folder", error_code);
						}
						//Handle if there already is a "New Folder \ (n)".
						else {
							size_t new_folder_number = 1;
							while (std::filesystem::exists(entry.path() / std::format("New Folder ({})", new_folder_number), error_code)) {
								new_folder_number++;
							}
							std::filesystem::create_directory(entry.path() / std::format("New Folder ({})", new_folder_number), error_code);
						}

						if (error_code) {
							LOG_WARN("Create \"New Folder\" error: {}", error_code.message());
						}
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				static std::string new_name;
				ImGui::InputText("##RenameInputFieldRecursive", &new_name);

				//Rename button.
				if (ImGui::MenuItem("Rename##Recursive") && !new_name.empty()) {
					std::error_code error_code;
					std::filesystem::rename(entry.path(), entry.path().parent_path() / new_name, error_code);

					if (error_code) {
						LOG_WARN("File rename operation error: {}", error_code.message());
					}

					new_name = "";
				}

				//Delete button.
				if (ImGui::MenuItem("Delete##Recursive")) {
					path_to_delete = entry.path();
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
				static std::string new_name;
				ImGui::InputText("##RenameInputFieldRecursive", &new_name);

				//Rename button.
				if (ImGui::MenuItem("Rename##Recursive") && !new_name.empty()) {
					std::filesystem::path rename_path(entry.path().parent_path());
					rename_path /= new_name;
					rename_path += entry.path().extension();

					std::error_code error_code;
					std::filesystem::rename(entry.path(), rename_path, error_code);

					if (error_code) {
						LOG_WARN("File rename operation error: {}", error_code.message());
					}

					new_name = "";
				}

				//Delete button.
				if (ImGui::MenuItem("Delete##Recursive")) {
					path_to_delete = entry.path();
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

		//If target is a folder moves the Payload to the folder, if target is a file, moves the Payload to the folder that file is in.
		if (ImGui::BeginDragDropTarget()) {
			if (auto payload = ImGui::AcceptDragDropPayload("PATH")) {
				std::filesystem::path payload_path(std::string{ static_cast<char*>(payload->Data), static_cast<std::string::size_type>(payload->DataSize) });

				std::filesystem::path new_path{};
				if (entry.is_directory()) {
					new_path = entry.path() / payload_path.filename();
				}
				else {
					new_path = entry.path().parent_path() / payload_path.filename();
				}

				std::error_code error_code;
				std::filesystem::rename(payload_path, new_path, error_code);

				if (error_code) {
					LOG_WARN("File move operation error: {}", error_code.message());
				}

				ImGui::EndDragDropTarget();
				if (open_node) {
					ImGui::TreePop();
				}
				break;
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
	ImGui::Begin("Resources##resources_ui");


	//DragDropTarget for resources window.
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


	//Add existing file button.
	if (ImGui::Button("Add File##resources_ui")) {
		if (auto add_path = open_file(window); add_path.has_value()) {
			std::error_code error_code;
			std::filesystem::copy_file(add_path.value(), base_path / add_path.value().filename(), error_code);

			if (error_code) {
				LOG_WARN("File copy operation error: {}", error_code.message());
			}
		}
	}


	ImGui::SameLine();


	//New folder button.
	if (ImGui::Button("New Folder##resources_ui")) {
		std::error_code error_code;

		//Create the new folder.
		if (!std::filesystem::exists(base_path / "New Folder", error_code)) {
			std::filesystem::create_directory(base_path / "New Folder", error_code);
		}
		//Handle if there already is a "New Folder \ (n)".
		else {
			size_t new_folder_number = 1;
			while (std::filesystem::exists(base_path / std::format("New Folder ({})", new_folder_number), error_code)) {
				new_folder_number++;
			}
			std::filesystem::create_directory(base_path / std::format("New Folder ({})", new_folder_number), error_code);
		}

		if (error_code) {
			LOG_WARN("Create \"New Folder\" error: {}", error_code.message());
		}
	}


	ImGui::Separator();


	recursive_node_create(base_path, window);


	ImGui::End();
}

//======================================================================================================================================================
//======================================================================================================================================================