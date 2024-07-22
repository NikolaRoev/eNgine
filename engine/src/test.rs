use core::{renderer::Renderer, scene_manager::{self, Scene, SceneManager, Sprite, Transform, ID}};
use std::{ sync::{Arc, Mutex, OnceLock}};

use wasm_bindgen::prelude::*;
use web_sys::{WebGl2RenderingContext, WebGlProgram, WebGlShader, console, WebGlUniformLocation};
use rhai::{run, Engine, EvalAltResult, Locked, Shared};
use std::cell::RefCell;
use std::rc::Rc;


use crate::{CAMERA_X, CAMERA_Y, X, Y};

thread_local! {
    pub static RENDERER: RefCell<Option<Renderer>> = RefCell::new(None);
}

pub fn init_render() {
    let _ = RENDERER.with(|e| {
        let document = web_sys::window().unwrap().document().unwrap();
        let canvas = document.get_element_by_id("canvas").unwrap();
        let canvas: web_sys::HtmlCanvasElement = canvas.dyn_into::<web_sys::HtmlCanvasElement>().unwrap();
        let context = canvas
            .get_context("webgl2").unwrap()
            .unwrap()
            .dyn_into::<WebGl2RenderingContext>().unwrap();
        context.viewport(0, 0, 1024, 576);
        e.replace(Some(Renderer::new(context)))
    });
}
pub fn add(id: &ID, sprite: &Sprite) {
    RENDERER.with(|e| {
        let mut borrow = e.borrow_mut();
        let ren = borrow.as_mut().unwrap();
        ren.add(id.id, sprite, 1024.0, 576.0);
    });
}

pub fn draw() {
    RENDERER.with(|e| {
        let mut borrow = e.borrow_mut();
        let ren = borrow.as_mut().unwrap();
        
        ren.context.clear_color(0.1, 0.1, 0.1, 1.0);
        ren.context.clear(WebGl2RenderingContext::COLOR_BUFFER_BIT);

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


                let model = glm::Mat4::identity();
                let model = glm::translate(&model, &glm::vec3(X as f32, Y as f32, 1.0));
                let model_location = ren.context.get_uniform_location(&ren.program.program, "model");
                ren.context.uniform_matrix4fv_with_f32_array(model_location.as_ref(), false, glm::value_ptr(&model));


                ren.context.draw_elements_with_f64(WebGl2RenderingContext::TRIANGLES, 6, WebGl2RenderingContext::UNSIGNED_INT, 0.0);
            }
        }
    });
}
