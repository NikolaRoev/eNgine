# CMakeList.txt : CMake project for eNgine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("eNgine")

file(GLOB IMGUI_SRC
     "vendor/imgui/*.h"
     "vendor/imgui/*.cpp"
)

add_library(imgui STATIC
    ${IMGUI_SRC}
    
    "vendor/imgui/misc/cpp/imgui_stdlib.cpp"
    "vendor/imgui/misc/cpp/imgui_stdlib.h"
)

# Add source to this project's executable.
add_executable(eNgine
    "src/eNgine.cpp"
    "src/eNgine.h"
    "src/ui.cpp"
    "src/ui.h"
    "vendor/glad/src/glad.c"
    "vendor/imgui/backends/imgui_impl_glfw.cpp"
    "vendor/imgui/backends/imgui_impl_glfw.h"
    "vendor/imgui/backends/imgui_impl_opengl3.cpp"
    "vendor/imgui/backends/imgui_impl_opengl3.h"
  "src/scene_manager.h"
 "src/scene_manager.cpp" "src/event_manager.h" "src/event_manager.cpp" "src/utility/uuid.h" "src/utility/uuid.cpp" "src/api.h" "src/resource_manager.h" "src/resource_manager.cpp" "src/renderer.h" "src/pch.h" "src/renderer.cpp" "src/window.h" "src/window.cpp")


include_directories(SYSTEM
    "vendor/glad/include"
    "vendor/imgui"
    "vendor/imgui/backends"
    "vendor/stb_image"
    "vendor/glm"
    "vendor/pybind11/include"
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("vendor/glfw")
add_subdirectory("vendor/spdlog")
add_subdirectory("vendor/pybind11")
#add_subdirectory("vendor/glm")

target_link_libraries(eNgine
    glfw
    imgui
    spdlog
    pybind11::embed
    #glm::glm
)


set_property(TARGET eNgine PROPERTY CXX_STANDARD 20)

# TODO: Add tests and install targets if needed.
