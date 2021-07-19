#pragma once
#include "pch.h"

//======================================================================================================================================================

class ResourceManager {
private:
	std::unordered_map<std::string, sf::Texture> textures;
	std::unordered_map<std::string, sf::Font> fonts;
	std::unordered_map<std::string, sf::SoundBuffer> sound_buffers;
	std::unordered_map<std::string, sf::Music> music;

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

	sf::Texture* get_texture(const std::string& name);
	sf::Font* get_font(const std::string& name);
	sf::SoundBuffer* get_sound_buffer(const std::string& name);
	sf::Music* get_music(const std::string& name);
};

//======================================================================================================================================================