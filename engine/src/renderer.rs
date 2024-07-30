use wasm_bindgen::prelude::*;
use std::{cell::RefCell, collections::HashMap};
use web_sys::{console, WebGl2RenderingContext, WebGlProgram, WebGlVertexArrayObject};


use crate::{program::Program, scene_manager::{Scene, SceneManager, Sprite, Transform, ID}};
use crate::{CAMERA_X, CAMERA_Y, X, Y};

extern crate nalgebra_glm as glm;



thread_local! {
    pub static RENDERER: RefCell<Option<Renderer>> = const { RefCell::new(None) };
}



pub struct Renderer {
    pub context: WebGl2RenderingContext,
    pub program: Program,
    pub vao_map: HashMap<uuid::Uuid, WebGlVertexArrayObject>
}


impl Renderer {
    fn new(context: WebGl2RenderingContext) -> Self {
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


    pub fn init() {
        let _ = RENDERER.with(|e| {
            let document = web_sys::window().unwrap().document().unwrap();
            let canvas = document.get_element_by_id("canvas").unwrap();
            let canvas: web_sys::HtmlCanvasElement = canvas.dyn_into::<web_sys::HtmlCanvasElement>().unwrap();
            let parent = canvas.parent_element().unwrap();
            let width = parent.client_width();
            let height = parent.client_height();

            println!("{width} x {height}");

            canvas.set_width(width.try_into().unwrap());
            canvas.set_height(height.try_into().unwrap());
            
            
            
            let context = canvas
                .get_context("webgl2").unwrap()
                .unwrap()
                .dyn_into::<WebGl2RenderingContext>().unwrap();
            context.viewport(0, 0, width, height);
            e.replace(Some(Renderer::new(context)))
        });
    }

    pub fn add_sprite(id: uuid::Uuid, sprite: &Sprite, window_width: f32, window_height: f32) {
        RENDERER.with(|e| {
            let mut borrow = e.borrow_mut();
            let renderer = borrow.as_mut().unwrap();
            
            
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

            let vao: web_sys::WebGlVertexArrayObject = renderer.context.create_vertex_array().unwrap();
            renderer.context.bind_vertex_array(Some(&vao));


            let position_attribute_location = renderer.context.get_attrib_location(&renderer.program.program, "position");

            let buffer = renderer.context.create_buffer().ok_or("Failed to create buffer").unwrap();
            renderer.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&buffer));
            let positions_array_buf_view = unsafe { js_sys::Float32Array::view(&vertices) };
            renderer.context.buffer_data_with_array_buffer_view(
                WebGl2RenderingContext::ARRAY_BUFFER,
                &positions_array_buf_view,
                WebGl2RenderingContext::STATIC_DRAW,
            );

            let el_buffer = renderer.context.create_buffer().ok_or("Failed to create buffer").unwrap();
            renderer.context.bind_buffer(WebGl2RenderingContext::ELEMENT_ARRAY_BUFFER, Some(&el_buffer));
            let in_array_buf_view = unsafe { js_sys::Int32Array::view(&indices) };
            renderer.context.buffer_data_with_array_buffer_view(
                WebGl2RenderingContext::ELEMENT_ARRAY_BUFFER,
                &in_array_buf_view,
                WebGl2RenderingContext::STATIC_DRAW,
            );

            renderer.context.vertex_attrib_pointer_with_i32(
                position_attribute_location as u32,
                3,
                WebGl2RenderingContext::FLOAT,
                false,
                0,
                0,
            );
            renderer.context.enable_vertex_attrib_array(position_attribute_location as u32);


            renderer.vao_map.insert(id, vao);
            renderer.context.bind_vertex_array(None);
        });
    }

    
    pub fn draw() {
        RENDERER.with(|e| {
            let mut borrow = e.borrow_mut();
            let ren = borrow.as_mut().unwrap();
            
            ren.context.clear_color(0.1, 0.1, 0.1, 1.0);
            ren.context.clear(WebGl2RenderingContext::COLOR_BUFFER_BIT);

            let scenes = SceneManager::get_scenes();
    
            for (id, vao) in &ren.vao_map {
                unsafe {
                    ren.context.bind_vertex_array(Some(vao));
    
    
                    let view = glm::Mat4::identity();
                    let view = glm::translate(&view, &glm::vec3(CAMERA_X, CAMERA_Y, 0.0));
                    let view_location = ren.context.get_uniform_location(&ren.program.program, "view");
                    ren.context.uniform_matrix4fv_with_f32_array(view_location.as_ref(), false, glm::value_ptr(&view));
    
                    let projection = glm::ortho(0.0, 1024.0, 0.0, 576.0, -100.0, 100.0);
                    let projection_location = ren.context.get_uniform_location(&ren.program.program, "projection");
                    ren.context.uniform_matrix4fv_with_f32_array(projection_location.as_ref(), false, glm::value_ptr(&projection));
    

                    let transform: Vec<Scene> = scenes.values().cloned().collect();
                    let transform = transform.first().unwrap();
                    let transform = transform.sprites.get(id).unwrap();
                    let transform = transform.transform.clone();


    
                    let model = glm::Mat4::identity();
                    let model = glm::translate(&model, &transform.position);
                    let model_location = ren.context.get_uniform_location(&ren.program.program, "model");
                    ren.context.uniform_matrix4fv_with_f32_array(model_location.as_ref(), false, glm::value_ptr(&model));
    
    
                    ren.context.draw_elements_with_f64(WebGl2RenderingContext::TRIANGLES, 6, WebGl2RenderingContext::UNSIGNED_INT, 0.0);
                }
            }
        });
    }
}
