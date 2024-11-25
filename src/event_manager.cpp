#include "event_manager.h"

std::queue<std::shared_ptr<Event>> EventManager::events;

void EventManager::add(Event* event) {
	events.emplace(event);
}

void EventManager::handle() {
	while (!events.empty()) {
		events.front()->handle();
		events.pop();
	}
}
