#pragma once
#include "pch.h"



struct ID {
	UUID uuid;
	std::string name;

	ID(const UUID& uuid, std::string name) : uuid{ uuid }, name{ name } {}
};

struct Transform {
	glm::vec3 position;
	glm::vec2 scale;
	float rotation;
};

class Sprite {
public:
	ID id;
	glm::vec2 size;
	Transform transform;
	std::optional<std::string> script;
	std::optional<std::string> texture;

	Sprite(
		const UUID& uuid,
		const std::string& name,
		const glm::vec2 size = glm::vec2(100.0f, 50.0f),
		const glm::vec3 position = glm::vec3(0.0f),
		const glm::vec2 scale = glm::vec2(0.0f),
		float rotation = 0,
		const std::optional<std::string>& texture = std::nullopt
	) : id{ uuid, name }, size{ size } {
		transform.position = position;
		transform.scale = scale;
		transform.rotation = rotation;
		this->texture = texture;
	}

	void set_name(const std::string& name) { this->id.name = name; }
	void set_width(float width) { this->size.x = width; }
	void set_height(float height) { this->size.y = height; }
	void set_x(float x) { this->transform.position.x = x; }
	void set_y(float y) { this->transform.position.y = y; }
	void set_z(float z) { this->transform.position.z = z; }
	void set_scale_x(float scale_x) { this->transform.scale.x = scale_x; }
	void set_scale_y(float scale_y) { this->transform.scale.y = scale_y; }
	void set_rotation(float rotation) { this->transform.rotation = rotation; }
	void set_script(const std::optional<std::string>& script) { this->script = script; }
	void set_texture(const std::optional<std::string>& texture) { this->texture = texture; }
};

class Scene {
public:
	ID id;
	std::string script;
	std::unordered_map<UUID, Sprite> sprites;


	Scene(const UUID& uuid, const std::string& name) : id{ uuid, name } {}


	

	void remove_sprite(const UUID& uuid) {
		sprites.erase(uuid);
	}

	Sprite& get_sprite(const UUID& uuid) {
		return sprites.at(uuid);
	}

	
};


class SceneManager {
public:
	static std::string active_scene;
	static std::unordered_map<UUID, Scene> scenes;

	

	static UUID add_scene(const std::string& name);

	static void remove_scene(const UUID& uuid);

	static Scene& get_scene(const UUID& uuid);

	static void set_name(const UUID& uuid, const std::string& name);

	static UUID add_sprite(const UUID& uuid, const std::string& name);

	static void remove_sprite(const UUID& uuid, const UUID& sprite_uuid);

	static void set_sprite_name(const UUID& uuid, const UUID& sprite_uuid, const std::string name);
	static void set_sprite_size(const UUID& uuid, const UUID& sprite_uuid, const float width, const float height);
	static void set_sprite_position(const UUID& uuid, const UUID& sprite_uuid, const float x, const float y);
	static void set_sprite_texture(const UUID& uuid, const UUID& sprite_uuid, const std::optional<std::string>& texture);
	static void set_sprite_script(const UUID& uuid, const UUID& sprite_uuid, const std::optional<std::string>& script);
};
