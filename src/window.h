#pragma once
#include "pch.h"

class Window {
public:
	static GLFWwindow* window;

	static void init();
	static void set_size(int width, int height);
	static void set_position(int x, int y);
};
