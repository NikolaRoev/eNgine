#include "pch.h"
#include "UI.h"

//======================================================================================================================================================

void resources_ui(ResourceManager& resource_manager) {
	ImGui::Begin("Resources");

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
