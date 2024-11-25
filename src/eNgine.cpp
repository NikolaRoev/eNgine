
#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ui.h"
#include "event_manager.h"
#include "api.h"
#include "renderer.h"
#include "window.h"


PYBIND11_EMBEDDED_MODULE(api, m) {
    m.def("add_scene", &add_scene);
    m.def("set_sprite_position", &set_sprite_position);
    m.def("key_pressed", &key_pressed);
}




int main() {
    pybind11::scoped_interpreter guard{};
    

    Window::init();
    Renderer::init();
    UI::init(Window::window);



    //auto scene = SceneManager::add_scene("test");
    //int count = 0;
    //for (int i = 0; i < 1000; i += 11) {
    //    for (int j = 0; j < 1000; j += 11) {
    //        auto sprite = SceneManager::add_sprite(scene, "sprite");
    //        SceneManager::set_sprite_size(scene, sprite, 10.0f, 10.0f);
    //        SceneManager::set_sprite_position(scene, sprite, (float)i, (float)j);
    //        SceneManager::set_sprite_texture(scene, sprite, "assets/textures/alien.png");
    //        Renderer::add_sprite(sprite, glm::vec2(10.0f, 10.0f));
    //
    //        count++;
    //    }
    //}
    //spdlog::info("SPRITES: {}", count);
    


    // render loop
    double delta_time = 0.0;
    double last_frame = 0.0;

    while (!glfwWindowShouldClose(Window::window)) {
        double current_frame = glfwGetTime();
        delta_time = (current_frame - last_frame) * 1000;
        last_frame = current_frame;

        glfwPollEvents();

        UI::new_frame();

        

        UI::render();
        UI::bind_framebuffer();

        
        

        glClearColor(0.0f, 0.0f, 0.0f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto& [scene_uuid, scene] : SceneManager::scenes) {
            for (auto& [sprite_uuid, sprite] : scene.sprites) {
                Renderer::draw(sprite);

                if (sprite.script) {
                    try {
                        auto test = pybind11::module_::import(sprite.script.value().c_str());
                        test.attr("on_frame")(scene_uuid.uuid, sprite_uuid.uuid, sprite.transform.position.x, sprite.transform.position.y, delta_time);
                    }
                    catch (pybind11::error_already_set e) {
                        spdlog::info("{}", e.what());
                    }
                }
            }
        }


        UI::unbind_framebuffer();
        UI::end_frame();

        glfwSwapBuffers(Window::window);
        

        EventManager::handle();
    }

    glfwDestroyWindow(Window::window);
    glfwTerminate();
    return 0;
}
