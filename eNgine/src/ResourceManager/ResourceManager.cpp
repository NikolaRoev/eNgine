#include "pch.h"
#include "ResourceManager.h"

//======================================================================================================================================================

void ResourceManager::add_texture(const std::filesystem::path& path) {
	const auto [it, result] = textures.emplace(std::piecewise_construct, std::forward_as_tuple(path), std::forward_as_tuple());
	if (result) {
		if (!it->second.loadFromFile(path.string())) {
			LOG_ERROR("Failed to load texture with file path [{}].", path.string());
		}
	}
	else {
		LOG_WARN("Texture with file path [{}] already exists.", path.string());
	}
}

//======================================================================================================================================================

void ResourceManager::add_font(const std::filesystem::path& path) {
	const auto [it, result] = fonts.emplace(std::piecewise_construct, std::forward_as_tuple(path), std::forward_as_tuple());
	if (result) {
		if (!it->second.loadFromFile(path.string())) {
			LOG_ERROR("Failed to load font with file path [{}].", path.string());
		}
	}
	else {
		LOG_WARN("Font with file path [{}] already exists.", path.string());
	}
}

//======================================================================================================================================================

void ResourceManager::add_sound_buffer(const std::filesystem::path& path) {
	const auto [it, result] = sound_buffers.emplace(std::piecewise_construct, std::forward_as_tuple(path), std::forward_as_tuple());
	if (result) {
		if (!it->second.loadFromFile(path.string())) {
			LOG_ERROR("Failed to load sound buffer with file path [{}].", path.string());
		}
	}
	else {
		LOG_WARN("Sound buffer with file path [{}] already exists.", path.string());
	}
}

//======================================================================================================================================================

void ResourceManager::add_music(const std::filesystem::path& path) {
	const auto [it, result] = music.emplace(std::piecewise_construct, std::forward_as_tuple(path), std::forward_as_tuple());

	if (result) {
		if (!it->second.openFromFile(path.string())) {
			LOG_ERROR("Failed to load music with file path [{}].", path.string());
		}
	}
	else {
		LOG_WARN("Music with file path [{}] already exists.", path.string());
	}
}

//======================================================================================================================================================