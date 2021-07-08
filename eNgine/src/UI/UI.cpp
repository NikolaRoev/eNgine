#include "pch.h"
#include "UI.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Utility.h"

//======================================================================================================================================================

void resources_ui(ResourceManager& resource_manager, sf::RenderWindow& window) {
	ImGui::Begin("Resources");

	if (ImGui::Button("ADD")) {
		if (auto file = open_file(window); file) {		
			auto path = std::filesystem::path(file.value());

			std::error_code error;

			if (path.extension().string() == ".png") {
				if (!std::filesystem::copy_file(file.value(), std::format("../assets/textures/{}", path.filename().string()), error)) {
					LOG_ERROR("Failed to add texture [{}]: [{}].", path.filename().string(), error.message());
				}
				else {
					resource_manager.add_texture(std::format("../assets/textures/{}", path.filename().string()));
				}
			}
			else if (path.extension().string() == ".ttf") {
				if (!std::filesystem::copy_file(file.value(), std::format("../assets/fonts/{}", path.filename().string()), error)) {
					LOG_ERROR("Failed to add font [{}]: [{}].", path.filename().string(), error.message());
				}
				else {
					resource_manager.add_font(std::format("../assets/fonts/{}", path.filename().string()));
				}

			}
			else if (path.extension().string() == ".wav") {
				if (!std::filesystem::copy_file(file.value(), std::format("../assets/sounds/{}", path.filename().string()), error)) {
					LOG_ERROR("Failed to add sound [{}]: [{}].", path.filename().string(), error.message());
				}
				else {
					resource_manager.add_sound_buffer(std::format("../assets/sounds/{}", path.filename().string()));
				}

			}
			else if (path.extension().string() == ".ogg") {
				if (!std::filesystem::copy_file(file.value(), std::format("../assets/music/{}", path.filename().string()), error)) {
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
	}

	ImGui::SameLine();

	if (ImGui::Button("REFRESH")) {

	}

	if (ImGui::TreeNode("Textures")) {
		for (const auto& it : resource_manager.textures) {
			ImGui::Text(it.first.filename().string().c_str());
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sounds")) {
		for (const auto& it : resource_manager.sound_buffers) {
			ImGui::Text(it.first.filename().string().c_str());
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Fonts")) {
		for (const auto& it : resource_manager.fonts) {
			ImGui::Text(it.first.filename().string().c_str());
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Music")) {
		for (const auto& it : resource_manager.music) {
			ImGui::Text(it.first.filename().string().c_str());
		}

		ImGui::TreePop();
	}

	ImGui::End();
}
