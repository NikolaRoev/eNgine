#pragma once
#include "pch.h"

//======================================================================================================================================================

class ResourceManager {
public:
	std::map<std::filesystem::path, sf::Texture> textures;
	std::map<std::filesystem::path, sf::Font> fonts;
	std::map<std::filesystem::path, sf::SoundBuffer> sound_buffers;
	std::map<std::filesystem::path, sf::Music> music;

public:
	ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	~ResourceManager() = default;


	void add_texture(const std::filesystem::path& path);
	void add_font(const std::filesystem::path& path);
	void add_sound_buffer(const std::filesystem::path& path);
	void add_music(const std::filesystem::path& path);
};

//======================================================================================================================================================