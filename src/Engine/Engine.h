#pragma once
#include "pch.h"
#include "ResourceManager/ResourceManager.h"
#include "Scene/Scene.h"

//======================================================================================================================================================

class Engine {
private:
	sf::RenderWindow window;
	sf::Event event{ sf::Event::MouseMoved };
	sf::Clock clock;


	ResourceManager resource_manager;

	std::unordered_map<std::string, Scene> scenes;
	Scene* current_scene{ nullptr };

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