#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <string>
#include <queue>
#include <memory>
#include <iostream>
#include <filesystem>
#include <format>
#include <mutex>
#include <optional>
#include <stdio.h>


#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi


#include <pybind11/embed.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"

#include "spdlog/spdlog.h"

#include "stb_image.h"

#include "utility/uuid.h"