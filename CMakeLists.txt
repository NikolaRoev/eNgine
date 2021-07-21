cmake_minimum_required(VERSION 3.10)

project(eNgine CXX)
set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/W4)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

add_executable(${PROJECT_NAME}
               src/pch.h
               src/Source.cpp
               src/Debug/Log.h
               src/Debug/Config.h
               src/Engine/Engine.h
               src/Engine/Engine.cpp
               src/ResourceManager/ResourceManager.h
               src/ResourceManager/ResourceManager.cpp
               src/ResourceManager/Utility.h
               src/ResourceManager/Utility.cpp
               src/UI/UI.h
               src/UI/UI.cpp
               src/Scene/Scene.h
               src/Scene/Scene.cpp
               
               vendor/imgui-1.83/imgui.cpp
               vendor/imgui-1.83/imgui_draw.cpp
               vendor/imgui-1.83/imgui_tables.cpp
               vendor/imgui-1.83/imgui_widgets.cpp
               vendor/imgui-1.83/imgui_demo.cpp
               vendor/imgui-1.83/misc/cpp/imgui_stdlib.cpp
               vendor/imgui-sfml-2.3/imgui-SFML.cpp
              )

target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)

add_compile_definitions(SFML_STATIC)

target_include_directories(${PROJECT_NAME} PRIVATE 
                           src
                           vendor
                           vendor/SFML-2.5.1/include
                           vendor/imgui-1.83
                           vendor/imgui-sfml-2.3
                           vendor/IconFontCppHeaders
                          )

target_link_directories(${PROJECT_NAME} PRIVATE vendor/SFML-2.5.1/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE
                      opengl32.lib
                      freetype.lib
                      winmm.lib
                      gdi32.lib
                      openal32.lib
                      flac.lib
                      vorbisenc.lib
                      vorbisfile.lib
                      vorbis.lib
                      ogg.lib
                      debug sfml-graphics-s-d.lib optimized sfml-graphics-s.lib
                      debug sfml-window-s-d.lib optimized sfml-window-s.lib
                      debug sfml-audio-s-d.lib optimized sfml-audio-s.lib
                      debug sfml-system-s-d.lib optimized sfml-system-s.lib
                     )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                        vendor/SFML-2.5.1/bin/openal32.dll
                        $<CONFIG>/openal32.dll
                   COMMENT "Copied openal32.dll.")