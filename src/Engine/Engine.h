#pragma once
#include "pch.h"
#include "ResourceManager/ResourceManager.h"

//======================================================================================================================================================

class Engine {
private:
	sf::RenderWindow window;
	sf::Event event{ sf::Event::MouseMoved };
	sf::Clock clock;

	ResourceManager resource_manager;

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