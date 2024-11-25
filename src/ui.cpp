#include "ui.h"
#include "scene_manager.h"
#include "event_manager.h"



static void recurRender(const std::filesystem::directory_entry& entry) {
    for (const auto& subEntry : std::filesystem::directory_iterator(entry)) {
        if (subEntry.is_directory()) {
            if (ImGui::TreeNodeEx(subEntry.path().stem().string().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                recurRender(subEntry);

                ImGui::TreePop();
            }
        }
        else {
            if (subEntry.path().extension().compare(".png") == 0) {
                auto filename = subEntry.path().filename().string();
                ImGui::Text(filename.c_str());
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                    auto path = subEntry.path().c_str();
                    ImGui::SetDragDropPayload("ASSET_PATH", path, (wcslen(path) + 1) * sizeof(std::filesystem::path::value_type));
                    ImGui::Text(filename.c_str());
                    ImGui::EndDragDropSource();
                }
            }
            if (subEntry.path().extension().compare(".py") == 0) {
                auto filename = subEntry.path().filename().string();
                auto stem = subEntry.path().stem().string();
                ImGui::Text(filename.c_str());
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                    auto path = std::format("assets.scripts.{}", stem);
                    
                    ImGui::SetDragDropPayload("SCRIPT_PATH", path.c_str(), (strlen(path.c_str()) + 1) * sizeof(char));
                    ImGui::Text(filename.c_str());
                    ImGui::EndDragDropSource();
                }
            }
        }
    }
}

void UI::assets() {
    std::string path = "assets";

    static std::once_flag init;
    std::call_once(init, [&path] { std::filesystem::create_directory(path); });

    ImGui::Begin("Assets");
    recurRender(std::filesystem::directory_entry(path));
    ImGui::End();
}

std::optional<UUID> selected_scene = std::nullopt;
std::optional<UUID> selected_sprite = std::nullopt;


bool renaming_started = false;
std::string new_name;
void UI::project_explorer() {
    ImGui::Begin("Project");



    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Add Scene")) {
            EventManager::add(new AddSceneEvent("Scene"));
        }
        ImGui::EndPopup();
    }

    const std::unordered_map<UUID, Scene> temp_scenes = SceneManager::scenes;
    for (const auto& [uuid, scene] : temp_scenes) {
        bool scene_open = ImGui::TreeNodeEx(std::format("{}###{}", scene.id.name, uuid.string()).c_str(), ImGuiTreeNodeFlags_DefaultOpen);

        std::string new_name = scene.id.name;
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::BeginMenu("Add")) {
                if (ImGui::MenuItem("Sprite")) {
                    EventManager::add(new AddSpriteEvent(uuid, "Sprite"));
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Rename###MenuItem")) {
                renaming_started = true;
            }
            if (ImGui::MenuItem("Delete")) {
                EventManager::add(new RemoveSceneEvent(uuid));
            }
            ImGui::EndPopup();
        }

        if (renaming_started) { ImGui::OpenPopup("Rename"); }
        if (ImGui::BeginPopup("Rename")) {
            ImGui::SetNextItemWidth(ImGui::GetFontSize() * 30);
            std::string new_name = scene.id.name;
            if (ImGui::InputText("###Name", &new_name, ImGuiInputTextFlags_EnterReturnsTrue)) {
                EventManager::add(new SetSceneNameEvent(uuid, new_name));
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetKeyboardFocusHere(-1);
            ImGui::EndPopup();
        }
        renaming_started = false;


        if (scene_open) {
            if (ImGui::TreeNodeEx(std::format("Sprites###{}", uuid.string()).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                for (auto& [sprite_uuid, sprite] : scene.sprites) {
                    if (ImGui::Button(std::format("{}###{}", sprite.id.name, sprite_uuid.string()).c_str())) {
                        selected_scene = uuid;
                        selected_sprite = sprite_uuid;
                    }
                    if (ImGui::BeginDragDropTarget()) {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_PATH")) {
                            auto data = static_cast<std::filesystem::path::value_type*>(payload->Data);
                            EventManager::add(new SetSpriteTextureEvent(uuid, sprite_uuid, std::filesystem::path(data).string()));
                        }
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCRIPT_PATH")) {
                            auto data = static_cast<char*>(payload->Data);
                            spdlog::info("{}", data);
                            EventManager::add(new SetSpriteScriptEvent(uuid, sprite_uuid, data));
                        }
                        ImGui::EndDragDropTarget();
                    }

                    if (ImGui::BeginPopupContextItem()) {
                        if (ImGui::MenuItem("Delete###Sprite")) {
                            EventManager::add(new RemoveSpriteEvent(uuid, sprite_uuid));
                        }
                        ImGui::EndPopup();
                    }
                }

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
    }



    ImGui::End();
}

void properties() {
    ImGui::Begin("Properties");
    if (selected_scene && selected_sprite) {
        Sprite sprite = SceneManager::get_scene(selected_scene.value()).get_sprite(selected_sprite.value());
        if (ImGui::InputText(std::format("SpriteName###{}", sprite.id.uuid.string()).c_str(), &sprite.id.name)) {
            EventManager::add(new SetSpriteNameEvent(selected_scene.value(), selected_sprite.value(), sprite.id.name));
        }

        if (ImGui::InputFloat2("Size", glm::value_ptr(sprite.size))) {
            EventManager::add(new SetSpriteSizeEvent(selected_scene.value(), selected_sprite.value(), sprite.size.x, sprite.size.y));
        }

        if (ImGui::InputFloat3("Position", glm::value_ptr(sprite.transform.position))) {
            EventManager::add(new SetSpritePositionEvent(selected_scene.value(), selected_sprite.value(), sprite.transform.position.x, sprite.transform.position.y));
        }

        ImGui::LabelText("Texture", "%s", sprite.texture ? sprite.texture.value().c_str() : "NONE");
        ImGui::LabelText("Script", "%s", sprite.script ? sprite.script.value().c_str() : "NONE");
    }
    ImGui::End();
}

static unsigned int FBO;
static unsigned int texture_id;
static unsigned int RBO;

void UI::preview() {
    ImGui::Begin("Preview");

    const float window_width = ImGui::GetContentRegionAvail().x;
    const float window_height = ImGui::GetContentRegionAvail().y;

    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImGui::GetWindowDrawList()->AddImage(
        (void*)texture_id,
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + window_width, pos.y + window_height),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImGui::End();
}

static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void UI::temp() {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;


    bool open = true;
    ImGui::Begin("DockSpace Demo", &open, window_flags);
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    

    ImGui::End();

    ImGui::Begin("Data");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}



void UI::init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);


    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
    
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void UI::new_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::render() {
    temp();
    assets();
    project_explorer();
    preview();
    properties();

    ImGui::Render();
}

void UI::end_frame() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
}

void UI::bind_framebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void UI::unbind_framebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
