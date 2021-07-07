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

			try {
				std::filesystem::copy_file(file.value(), std::format("../assets/{}", path.filename().string()));
			}
			catch (std::filesystem::filesystem_error& error) {
				LOG_ERROR(error.what());
			}
		}
	}

	if (ImGui::TreeNode("Textures")) {

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sounds")) {

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Fonts")) {

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Music")) {

		ImGui::TreePop();
	}

	ImGui::End();
}
