#pragma once
#include "pch.h"

//======================================================================================================================================================

class Engine {
private:
	sf::RenderWindow window;
	sf::Event event{ sf::Event::MouseMoved };
	sf::Clock clock;

public:
	Engine();
	Engine(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine& operator=(Engine&&) = delete;
	~Engine();
	

	void loop();
};

//======================================================================================================================================================