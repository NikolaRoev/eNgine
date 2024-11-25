#include "renderer.h"
#include "resource_manager.h"

std::unordered_map<UUID, unsigned int> Renderer::vaos;
unsigned int Renderer::program;



const char* vertex_shader = R"*(
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
    
void main() {
            
    gl_Position = projection * view * model * vec4(position, 1.0);
    TexCoord = aTexCoord;
}
)*";

const char* fragment_shader = R"*(
#version 330

out vec4 outColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
            
void main() {
    outColor = texture(ourTexture, TexCoord);
}
)*";



void check_compile_errors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n" << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n" << std::endl;
        }
    }
}


void Renderer::init() {
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");
    // shader Program
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    check_compile_errors(program, "PROGRAM");
    // delete the shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glUseProgram(program);
}

void Renderer::draw(const Sprite& sprite) {
    auto id = vaos.at(sprite.id.uuid);
    glBindVertexArray(id);
    auto view1 = glm::mat4(1.0f);
    auto view = glm::translate(view1, glm::vec3(0.0f, 0.0f, 0.0f));
    auto view_location = glGetUniformLocation(program, "view");
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
    
    auto projection = glm::ortho(0.0f, 1024.0f, 0.0f, 576.0f, -100.0f, 100.0f);
    auto projection_location = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
    
    auto model1 = glm::mat4(1.0f);
    auto model = glm::translate(model1, sprite.transform.position);
    auto model_location = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
    
    
    if (sprite.texture) {
        ResourceManager::get(sprite.texture.value()).bind();
    }
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::add_sprite(const UUID uuid, glm::vec2 size) {
	float vertices[] = {
		size.x, size.y, 0.0f, 1.0f, 1.0f, // Top right.
		size.x, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom right.
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom left.
		0.0f, size.y, 0.0f, 0.0f, 1.0f // Top left.
	};
	int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};



    unsigned int VBO, IBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    vaos[uuid] = VAO;
}
