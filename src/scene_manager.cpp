#include "scene_manager.h"
#include "renderer.h"

std::string SceneManager::active_scene;
std::unordered_map<UUID, Scene> SceneManager::scenes;

UUID SceneManager::add_scene(const std::string& name) {
	UUID uuid = UUID();
	scenes.emplace(std::piecewise_construct, std::forward_as_tuple(uuid), std::forward_as_tuple(uuid, name));
	return uuid;
}

void SceneManager::remove_scene(const UUID& uuid) {
	scenes.erase(uuid);
}

Scene& SceneManager::get_scene(const UUID& uuid) {
	return scenes.at(uuid);
}

void SceneManager::set_name(const UUID& uuid, const std::string& name) {
	scenes.at(uuid).id.name = name;
}

UUID SceneManager::add_sprite(const UUID& uuid, const std::string& name) {
	auto& scene = scenes.at(uuid);
	UUID sprite_uuid = UUID();
	scene.sprites.emplace(std::piecewise_construct, std::forward_as_tuple(sprite_uuid), std::forward_as_tuple(sprite_uuid, name));
	Renderer::add_sprite(sprite_uuid);
	return sprite_uuid;
}

void SceneManager::remove_sprite(const UUID& uuid, const UUID& sprite_uuid) {
	auto& scene = scenes.at(uuid);
	scene.sprites.erase(sprite_uuid);
}

void SceneManager::set_sprite_name(const UUID& uuid, const UUID& sprite_uuid, const std::string name) {
	auto& scene = scenes.at(uuid);
	auto& sprite = scene.sprites.at(sprite_uuid);
	sprite.set_name(name);
}

void SceneManager::set_sprite_size(const UUID& uuid, const UUID& sprite_uuid, const float width, const float height)	{
	auto& scene = scenes.at(uuid);
	auto& sprite = scene.sprites.at(sprite_uuid);
	sprite.set_width(width);
	sprite.set_height(height);
}

void SceneManager::set_sprite_position(const UUID& uuid, const UUID& sprite_uuid, const float x, const float y) {
	auto& scene = scenes.at(uuid);
	auto& sprite = scene.sprites.at(sprite_uuid);
	sprite.set_x(x);
	sprite.set_y(y);
}

void SceneManager::set_sprite_texture(const UUID& uuid, const UUID& sprite_uuid, const std::optional<std::string>& texture) {
	auto& scene = scenes.at(uuid);
	auto& sprite = scene.sprites.at(sprite_uuid);
	sprite.set_texture(texture);
}

void SceneManager::set_sprite_script(const UUID& uuid, const UUID& sprite_uuid, const std::optional<std::string>& script) {
	auto& scene = scenes.at(uuid);
	auto& sprite = scene.sprites.at(sprite_uuid);
	sprite.set_script(script);
}
