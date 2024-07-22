use core::{renderer::Renderer, scene_manager::{self, Scene, SceneManager, Sprite, Transform, ID}};
use std::{ sync::{Arc, Mutex, OnceLock}};

use test::{add, draw, init_render};
use wasm_bindgen::prelude::*;
use web_sys::{WebGl2RenderingContext, WebGlProgram, WebGlShader, console, WebGlUniformLocation};
use rhai::{run, Engine, EvalAltResult, Locked, Shared};
use std::cell::RefCell;
use std::rc::Rc;

mod test;


extern crate nalgebra_glm as glm;

static mut X: f64 = 0.0;
static mut Y: f64 = 0.0;

static mut CAMERA_X: f32 = 0.0;
static mut CAMERA_Y: f32 = 0.0;







#[wasm_bindgen(start)]
fn main() -> Result<(), JsValue> {
    //let engine = Engine::new();
    //let result = engine.eval::<i64>("40 + 3").unwrap();
    //console::log_1(&format!("Answer: {result}").into());


    let scene_name = "name".to_string();
    SceneManager::init();
    SceneManager::add_scene(&scene_name);
    let entity = SceneManager::add_entity(&scene_name, "entity 1").unwrap();
    SceneManager::add_sprite(&scene_name, &entity);
    SceneManager::add_transform(&scene_name, &entity);
    

    init_render();

    let binding = SceneManager::get_scenes();
    let scene = binding.first();
    let scene = scene.unwrap();

    for (id, sprite) in &scene.sprites {
        add(id, sprite);
    }
    
    console::log_1(&format!("saa dude").into());
    
    let f: std::rc::Rc<std::cell::RefCell<Option<Closure<dyn FnMut(f64)>>>> = std::rc::Rc::new(std::cell::RefCell::new(None));
    let outer_f: std::rc::Rc<std::cell::RefCell<Option<Closure<dyn FnMut(f64)>>>> = f.clone();

    let window = web_sys::window().unwrap();
    if let Some(perf) = window.performance() {
        *outer_f.borrow_mut() = Some(Closure::wrap(Box::new(move |t: f64| {
            //let start_time = perf.now();
            //console::log_1(&format!("!!!!!: {t}").into());


            

            draw();
            
            

            
            //let end_time = perf.now();
            //console::log_1(&format!("FRAME TIME 1: {}", end_time - start_time).into());
            window.request_animation_frame(f.borrow().as_ref().unwrap().as_ref().unchecked_ref())
                .expect("failed requesting animation frame");
        }) as Box<dyn FnMut(f64)>));

        let window = web_sys::window().unwrap();
        window.request_animation_frame(outer_f.borrow().as_ref().unwrap().as_ref().unchecked_ref())
            .expect("failed requesting animation frame");

    }

    Ok(())
}

#[wasm_bindgen]
pub fn move_rect(x: f64, y: f64) {
    unsafe {
        X = x;
        Y = y;
    }
    
    //console::log_1(&format!("Move: {x} {y}").into());
}

#[wasm_bindgen]
pub fn move_cam(x: f32, y: f32) {
    unsafe {
        CAMERA_X = x;
        CAMERA_Y = y;
    }
}