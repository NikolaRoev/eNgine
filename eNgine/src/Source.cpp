#include "pch.h"


int main() {
	LOG_INIT();
	
	sf::RenderWindow window;
	window.create(sf::VideoMode{ 1024, 576, 32 }, "H Adventure");
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);



	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);



	sf::Clock deltaClock;
	sf::Event event;
	while (window.isOpen()) {
		window.clear();



		ImGui::SFML::Update(window, deltaClock.restart());


		ImGui::Begin("Hello, world!");
		ImGui::Button("Look at this pretty button");
		ImGui::End();


		



		window.draw(shape);




		ImGui::SFML::Render(window);
		window.display();


		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
	}


	ImGui::SFML::Shutdown();

	LOG_DEINIT();
}