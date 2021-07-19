#include "pch.h"
#include "Engine/Engine.h"

int main() {
	LOG_INIT();
	
	Engine* engine = new Engine();
	engine->loop();
	delete engine;

	LOG_DEINIT();
}