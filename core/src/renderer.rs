use std::collections::HashMap;
use web_sys::{WebGl2RenderingContext, WebGlProgram, WebGlVertexArrayObject};


use crate::{program::Program, scene_manager::{Sprite, ID}};

extern crate nalgebra_glm as glm;


pub struct Renderer {
    pub context: WebGl2RenderingContext,
    pub program: Program,
    pub vao_map: HashMap<uuid::Uuid, WebGlVertexArrayObject>
}


impl Renderer {
    pub fn new(context: WebGl2RenderingContext) -> Self {
        let program = Program::create(
            &context,
            r##"#version 300 es
     
            in vec4 position;
    
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
    
            void main() {
            
                gl_Position = projection * view * model * position;
            }
            "##,
            r##"#version 300 es
        
            precision highp float;
            out vec4 outColor;
            
            void main() {
                outColor = vec4(0, 0, 0, 0);
            }
            "##
        ).unwrap();
        program.bind(&context);
        
        Renderer { context, program, vao_map: HashMap::default() }
    }

    pub fn add(&mut self, id: uuid::Uuid, sprite: &Sprite, window_width: f32, window_height: f32) {
        let width = window_width * sprite.scale.x;
        let height = window_height * sprite.scale.y;

        let vertices: [f32; 12] = [
            width, height, 0.0, // Top right.
            width,    0.0, 0.0, // Bottom right.
            0.0,    0.0, 0.0, // Bottom left.
            0.0, height, 0.0  // Top left.
        ];
        let indices: [i32; 6] = [
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        ];

        let vao: web_sys::WebGlVertexArrayObject = self.context.create_vertex_array().unwrap();
        self.context.bind_vertex_array(Some(&vao));


        let position_attribute_location = self.context.get_attrib_location(&self.program.program, "position");

        let buffer = self.context.create_buffer().ok_or("Failed to create buffer").unwrap();
        self.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&buffer));
        let positions_array_buf_view = unsafe { js_sys::Float32Array::view(&vertices) };
        self.context.buffer_data_with_array_buffer_view(
            WebGl2RenderingContext::ARRAY_BUFFER,
            &positions_array_buf_view,
            WebGl2RenderingContext::STATIC_DRAW,
        );

        let el_buffer = self.context.create_buffer().ok_or("Failed to create buffer").unwrap();
        self.context.bind_buffer(WebGl2RenderingContext::ELEMENT_ARRAY_BUFFER, Some(&el_buffer));
        let in_array_buf_view = unsafe { js_sys::Int32Array::view(&indices) };
        self.context.buffer_data_with_array_buffer_view(
            WebGl2RenderingContext::ELEMENT_ARRAY_BUFFER,
            &in_array_buf_view,
            WebGl2RenderingContext::STATIC_DRAW,
        );

        self.context.vertex_attrib_pointer_with_i32(
            position_attribute_location as u32,
            3,
            WebGl2RenderingContext::FLOAT,
            false,
            0,
            0,
        );
        self.context.enable_vertex_attrib_array(position_attribute_location as u32);


        self.vao_map.insert(id, vao);
        self.context.bind_vertex_array(None);
    }
}
