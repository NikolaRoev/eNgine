#pragma once
#include "pch.h"


class UI {
private:
	static void assets();
	static void project_explorer();
	static void preview();
	static void temp();

public:
	static void init(GLFWwindow* window);
	static void new_frame();
	static void render();
	static void end_frame();

	static void bind_framebuffer();
	static void unbind_framebuffer();
};
