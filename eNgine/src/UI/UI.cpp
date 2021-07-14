#include "pch.h"
#include "UI.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Utility.h"

//======================================================================================================================================================
//======================================================================================================================================================

void load_resource(ResourceManager& resource_manager, const std::filesystem::path& path) {
	std::error_code error;

	if (path.extension().string() == ".png") {
		if (!std::filesystem::copy_file(path, std::format("../assets/textures/{}", path.filename().string()), error)) {
			LOG_ERROR("Failed to add texture [{}]: [{}].", path.filename().string(), error.message());
		}
		else {
			resource_manager.add_texture(std::format("../assets/textures/{}", path.filename().string()));
		}
	}
	else if (path.extension().string() == ".ttf") {
		if (!std::filesystem::copy_file(path, std::format("../assets/fonts/{}", path.filename().string()), error)) {
			LOG_ERROR("Failed to add font [{}]: [{}].", path.filename().string(), error.message());
		}
		else {
			resource_manager.add_font(std::format("../assets/fonts/{}", path.filename().string()));
		}

	}
	else if (path.extension().string() == ".wav") {
		if (!std::filesystem::copy_file(path, std::format("../assets/sounds/{}", path.filename().string()), error)) {
			LOG_ERROR("Failed to add sound [{}]: [{}].", path.filename().string(), error.message());
		}
		else {
			resource_manager.add_sound_buffer(std::format("../assets/sounds/{}", path.filename().string()));
		}

	}
	else if (path.extension().string() == ".ogg") {
		if (!std::filesystem::copy_file(path, std::format("../assets/music/{}", path.filename().string()), error)) {
			LOG_ERROR("Failed to add music [{}]: [{}].", path.filename().string(), error.message());
		}
		else {
			resource_manager.add_music(std::format("../assets/music/{}", path.filename().string()));
		}

	}
	else {
		LOG_ERROR("Unsupported file format [{}].", path.extension().string());
	}
}

//======================================================================================================================================================

void add_resource(ResourceManager& resource_manager, const std::filesystem::path& path) {
	std::error_code error;

	if (path.extension().string() == ".png") {
		resource_manager.add_texture(path);
	}
	else if (path.extension().string() == ".ttf") {
		resource_manager.add_font(path);
	}
	else if (path.extension().string() == ".wav") {
		resource_manager.add_sound_buffer(path);
	}
	else if (path.extension().string() == ".ogg") {
		resource_manager.add_music(path);
	}
	else {
		LOG_ERROR("Unsupported file format [{}].", path.extension().string());
	}
}

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
				std::filesystem::path payload_path(std::string{ (char*)payload->Data, (std::string::size_type)payload->DataSize });

				//TO DO: Handle if dragdrop source and dragdrop target are either file or directory. Good luck lul.
			}

			ImGui::EndDragDropTarget();
		}
	}
}

void resources_ui(ResourceManager& resource_manager, sf::RenderWindow& window) {
	ImGui::Begin("Resources");


	if (ImGui::Button("ADD")) {
		if (auto file = open_file(window); file) {		
			auto path = std::filesystem::path(file.value());
			load_resource(resource_manager, path);
		}
		//TO DO: Rework this.
	}

	ImGui::Separator();

	recursive_node_create("../assets");

	ImGui::End();
}

//======================================================================================================================================================
//======================================================================================================================================================