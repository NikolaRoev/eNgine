#include "resource_manager.h"


std::unordered_map<std::string, Texture> ResourceManager::textures;


void ResourceManager::add_texture(const std::string& path) {
	textures.emplace(std::piecewise_construct, std::forward_as_tuple(path), std::forward_as_tuple(path));
}

Texture& ResourceManager::get(const std::string& path) {
	if (textures.find(path) == textures.end()) {
		add_texture(path);
	}
	return textures.at(path);
}
