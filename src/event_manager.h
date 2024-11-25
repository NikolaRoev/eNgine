#pragma once
#include "pch.h"
#include "scene_manager.h"
#include "renderer.h"


class Event {
public:
	Event() = default;
	virtual void handle() = 0;
};



class AddSceneEvent : public Event {
private:
	std::string name;

public:
	AddSceneEvent(const std::string& name) : name{ name } {}

	virtual void handle() override {
		SceneManager::add_scene(this->name);
	}
};

class RemoveSceneEvent : public Event {
private:
	UUID uuid;

public:
	RemoveSceneEvent(const UUID& uuid) : uuid{ uuid } {}

	virtual void handle() override {
		SceneManager::remove_scene(this->uuid);
	}
};

class SetSceneNameEvent : public Event {
private:
	UUID uuid;
	std::string name;

public:
	SetSceneNameEvent(const UUID uuid, const std::string& name) : name{ name }, uuid{ uuid } {}

	virtual void handle() override {
		SceneManager::set_name(this->uuid, this->name);
	}
};

class AddSpriteEvent : public Event {
private:
	UUID uuid;
	std::string name;

public:
	AddSpriteEvent(const UUID uuid, const std::string& name) : name{ name }, uuid{ uuid } {}

	virtual void handle() override {
		SceneManager::add_sprite(this->uuid, this->name);
	}
};

class RemoveSpriteEvent : public Event {
private:
	UUID scene_uuid;
	UUID sprite_uuid;

public:
	RemoveSpriteEvent(const UUID scene_uuid, const UUID sprite_uuid) : scene_uuid{ scene_uuid }, sprite_uuid{ sprite_uuid } {}

	virtual void handle() override {
		SceneManager::remove_sprite(this->scene_uuid, this->sprite_uuid);
	}
};

class SetSpriteNameEvent : public Event {
private:
	UUID scene_uuid;
	UUID sprite_uuid;
	std::string name;

public:
	SetSpriteNameEvent(const UUID scene_uuid, const UUID sprite_uuid, const std::string& name) : scene_uuid{ scene_uuid }, sprite_uuid{ sprite_uuid }, name{ name } {}

	virtual void handle() override {
		SceneManager::set_sprite_name(this->scene_uuid, this->sprite_uuid, this->name);
	}
};

class SetSpriteSizeEvent : public Event {
private:
	UUID scene_uuid;
	UUID sprite_uuid;
	float width;
	float height;

public:
	SetSpriteSizeEvent(const UUID scene_uuid, const UUID sprite_uuid, const float width, const float height) : scene_uuid{ scene_uuid }, sprite_uuid{ sprite_uuid }, width{ width }, height{ height } {}

	virtual void handle() override {
		SceneManager::set_sprite_size(this->scene_uuid, this->sprite_uuid, width, height);
		Renderer::add_sprite(this->sprite_uuid, glm::vec2(width, height));
	}
};

class SetSpritePositionEvent : public Event {
private:
	UUID scene_uuid;
	UUID sprite_uuid;
	float x;
	float y;

public:
	SetSpritePositionEvent(const UUID scene_uuid, const UUID sprite_uuid, const float x, const float y) : scene_uuid{ scene_uuid }, sprite_uuid{ sprite_uuid }, x{ x }, y{ y } {}

	virtual void handle() override {
		SceneManager::set_sprite_position(this->scene_uuid, this->sprite_uuid, x, y);
	}
};

class SetSpriteTextureEvent : public Event {
private:
	UUID scene_uuid;
	UUID sprite_uuid;
	std::optional<std::string> path;

public:
	SetSpriteTextureEvent(const UUID scene_uuid, const UUID sprite_uuid, const std::optional<std::string>& path) : scene_uuid{ scene_uuid }, sprite_uuid{ sprite_uuid }, path{ path } {}

	virtual void handle() override {
		SceneManager::set_sprite_texture(this->scene_uuid, this->sprite_uuid, this->path);
	}
};

class SetSpriteScriptEvent : public Event {
private:
	UUID scene_uuid;
	UUID sprite_uuid;
	std::optional<std::string> path;

public:
	SetSpriteScriptEvent(const UUID scene_uuid, const UUID sprite_uuid, const std::optional<std::string>& path) : scene_uuid{ scene_uuid }, sprite_uuid{ sprite_uuid }, path{ path } {}

	virtual void handle() override {
		SceneManager::set_sprite_script(this->scene_uuid, this->sprite_uuid, this->path);
	}
};



class EventManager {
private:
	static std::queue<std::shared_ptr<Event>> events;

public:
	static void add(Event* event);
	static void handle();
};
