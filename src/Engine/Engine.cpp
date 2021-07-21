#include "pch.h"
#include "Engine.h"
#include "ResourceManager/ResourceManager.h"
#include "UI/UI.h"

//======================================================================================================================================================
//======================================================================================================================================================

Engine::Engine() {
	std::filesystem::create_directory(base_path);

	window.create(sf::VideoMode{ 1360, 768, 32 }, "eNgine");
	window.setFramerateLimit(60);
	
	ImGui::SFML::Init(window, false);


	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 16.0f, &icons_config, icons_ranges);

	ImGui::SFML::UpdateFontTexture();
}

//======================================================================================================================================================

Engine::~Engine() {
	ImGui::SFML::Shutdown();
}

//======================================================================================================================================================
//======================================================================================================================================================

void Engine::loop() {
	while (window.isOpen()) {
		window.clear();
		ImGui::SFML::Update(window, clock.restart());


		//UI:
		menu_bar_ui();
		resources_ui(resource_manager, window);

		//Draw:



		ImGui::SFML::Render(window);
		window.display();

		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
	}
}

//======================================================================================================================================================
//======================================================================================================================================================