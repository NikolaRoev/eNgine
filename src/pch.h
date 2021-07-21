#pragma once

//======================================================================================================================================================

#ifdef _WIN32
	#include <Windows.h>
	#include <commdlg.h>
	#include <shellapi.h>

	#undef min
	#undef max
#endif

//======================================================================================================================================================

#include "Debug/Config.h"

//======================================================================================================================================================

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <tuple>
#include <optional>
#include <utility>
#include <filesystem>
#include <format>

//======================================================================================================================================================

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/OpenGL.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui-SFML.h"

#include "IconsFontAwesome5.h"

//======================================================================================================================================================