#pragma once
#include "pch.h"

//======================================================================================================================================================

class ResourceManager {
public:
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Font> fonts;
	std::map<std::string, sf::SoundBuffer> sound_buffers;
	std::map<std::string, sf::Music> music;

public:
	ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	~ResourceManager() = default;


	void add_texture(const std::string& file_path);
	void add_font(const std::string& file_path);
	void add_sound_buffer(const std::string& file_path);
	void add_music(const std::string& file_path);
};

//======================================================================================================================================================