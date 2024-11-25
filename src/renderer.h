#pragma once
#include "pch.h"
#include "scene_manager.h"

class Renderer {
public:
	static unsigned int program;
	static std::unordered_map<UUID, unsigned int> vaos;

	static void init();
	static void draw(const Sprite& sprite);

	static void add_sprite(const UUID uuid, glm::vec2 size = glm::vec2(100.0f, 50.0f));
};