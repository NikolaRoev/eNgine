#pragma once
#include "pch.h"
#include "event_manager.h"
#include "scene_manager.h"
#include "window.h"



void add_scene(const std::string& name) {
	EventManager::add(new AddSceneEvent(name));
}

void add_sprite(const uint64_t scene_uuid, const std::string& name) {
	EventManager::add(new AddSpriteEvent(scene_uuid, name));
}

std::vector<std::string> get_sprites(const uint64_t scene_uuid, const std::string& name) {
	std::vector<std::string> out;
	for (const auto& [_, sprite] : SceneManager::get_scene(scene_uuid).sprites) {
		if (sprite.id.name == name) {
			out.push_back(sprite.id.uuid.string());
		}
	}
	return out;
}

void set_sprite_position(const uint64_t scene_uuid, const uint64_t sprite_uuid, const float x, const float y) {
	EventManager::add(new SetSpritePositionEvent(scene_uuid, sprite_uuid, x, y));
}

void set_sprite_texture(const uint64_t scene_uuid, const uint64_t sprite_uuid, const std::string& name) {
	EventManager::add(new SetSpriteTextureEvent(scene_uuid, sprite_uuid, name));
}

bool key_pressed(int key) {
	//GLFW_KEY_RIGHT              262
	//GLFW_KEY_LEFT               263
	//GLFW_KEY_DOWN               264
	//GLFW_KEY_UP                 265
	return glfwGetKey(Window::window, key) == GLFW_PRESS;
}
