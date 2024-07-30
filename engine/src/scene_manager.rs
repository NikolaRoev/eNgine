use wasm_bindgen::prelude::*;
use web_sys::console;

use std::collections::HashMap;
use std::cell::RefCell;
use serde::{Serialize, Deserialize};

use crate::renderer::Renderer;

extern crate nalgebra_glm as glm;



thread_local! {
    pub static SCENE_MANAGER: RefCell<Option<SceneManager>> = const { RefCell::new(None) };
}



#[wasm_bindgen(typescript_custom_section)]
const SCENE_MANAGER_TYPES: &'static str = r#"

export interface ID { uuid: string, name: string }
export interface Transform { position: [number, number, number], scale: [number, number], rotation: number }
export interface Sprite { id: ID, scale: [number, number], transform: Transform }

export interface Scene {
    id: ID,

    sprites: Map<string, Sprite>
}

export type Scenes = Map<string, Scene>;

"#;

#[wasm_bindgen]
extern "C" {
    #[wasm_bindgen(typescript_type = "ID")]
    pub type TID;
    #[wasm_bindgen(typescript_type = "Transform")]
    pub type TTransform;
    #[wasm_bindgen(typescript_type = "Sprite")]
    pub type TSprite;
    

    #[wasm_bindgen(typescript_type = "Scene")]
    pub type TScene;

    #[wasm_bindgen(typescript_type = "Scenes")]
    pub type TScenes;
}



#[derive(Default, Clone, Serialize, Deserialize)]
pub struct ID { pub uuid: uuid::Uuid, pub name: String }

#[derive(Default, Clone, Serialize, Deserialize)]
pub struct Transform { pub position: glm::Vec3, pub scale: glm::Vec2, pub rotation: f32 }

#[derive(Default, Clone, Serialize, Deserialize)]
pub struct Sprite { pub id: ID, pub scale: glm::Vec2, pub transform: Transform }




#[derive(Default, Clone, Serialize, Deserialize)]
pub struct Scene {
    pub id: ID,
    
    pub sprites: HashMap<uuid::Uuid, Sprite>
}


#[derive(Default)]
pub struct SceneManager {
    pub scenes: HashMap<uuid::Uuid, Scene>
}


impl SceneManager {
    pub fn init() {
        let _ = SCENE_MANAGER.with(|e| {
            e.replace(Some(SceneManager::default()))
        });
    }

    pub fn get_scenes() -> HashMap<uuid::Uuid, Scene> {
        SCENE_MANAGER.with(|e| {
            let borrow = e.borrow();
            let scene_manager = borrow.as_ref().unwrap();

            scene_manager.scenes.clone()
        })
    }

    pub fn add_scene(name: &str) -> uuid::Uuid {
        SCENE_MANAGER.with(|e| {
            let mut borrow = e.borrow_mut();
            let scene_manager = borrow.as_mut().unwrap();

            let uuid = uuid::Uuid::new_v4();
            scene_manager.scenes.insert(uuid, Scene { id: ID { uuid, name: name.to_string() }, ..Default::default() });

            uuid
        })
    }


    pub fn add_sprite(scene_id: &uuid::Uuid, sprite_name: String) -> Option<uuid::Uuid> {
        SCENE_MANAGER.with(|e| {
            let mut borrow = e.borrow_mut();
            let scene_manager = borrow.as_mut().unwrap();

            
            if let Some(scene) = scene_manager.scenes.get_mut(scene_id) {
                let uuid = uuid::Uuid::new_v4();

                scene.sprites.insert(uuid, Sprite {
                    id: ID { uuid, name: sprite_name },
                    scale: glm::Vec2::new(0.1, 0.1),
                    ..Default::default()
                });

                return Some(uuid);
            }

            None
        })
    }

    pub fn set_sprite_scale(scene_id: &uuid::Uuid, sprite_id: &uuid::Uuid, scale_x: f32, scale_y: f32) {
        SCENE_MANAGER.with(|e| {
            let mut borrow = e.borrow_mut();
            let scene_manager = borrow.as_mut().unwrap();

            
            if let Some(scene) = scene_manager.scenes.get_mut(scene_id) {
                if let Some(sprite) = scene.sprites.get_mut(sprite_id) {
                    sprite.scale.x = scale_x;
                    sprite.scale.y = scale_y;
                }
            }
        });
    }

    pub fn set_sprite_position(scene_id: &uuid::Uuid, sprite_id: &uuid::Uuid, x: f32, y: f32, z: f32) {
        SCENE_MANAGER.with(|e| {
            let mut borrow = e.borrow_mut();
            let scene_manager = borrow.as_mut().unwrap();


            if let Some(scene) = scene_manager.scenes.get_mut(scene_id) {
                if let Some(sprite) = scene.sprites.get_mut(sprite_id) {
                    sprite.transform.position.x = x;
                    sprite.transform.position.y = y;
                    sprite.transform.position.z = z;
                }
            }
        });
    }
}




#[wasm_bindgen]
pub fn get_scenes() -> TScenes {
    TScenes::from(serde_wasm_bindgen::to_value(&SceneManager::get_scenes()).unwrap())
}

#[wasm_bindgen]
pub fn add_scene(name: &str) {
    SceneManager::add_scene(name);
}


#[wasm_bindgen]
pub fn add_sprite(scene_id: &str, sprite_name: &str) {
    let sprite_id = SceneManager::add_sprite(
        &uuid::Uuid::parse_str(scene_id).unwrap(), sprite_name.to_string()
    ).unwrap();
    Renderer::add_sprite(sprite_id, &Sprite { scale: glm::Vec2::new(0.1, 0.1), ..Default::default() }, 1024.0, 576.0);
}

#[wasm_bindgen]
pub fn set_sprite_scale(scene_id: &str, sprite_id: &str, scale_x: f32, scale_y: f32) {
    SceneManager::set_sprite_scale(
        &uuid::Uuid::parse_str(scene_id).unwrap(),
        &uuid::Uuid::parse_str(sprite_id).unwrap(),
        scale_x,
        scale_y
    );
}

#[wasm_bindgen]
pub fn set_sprite_position(scene_id: &str, sprite_id: &str, x: f32, y: f32, z: f32) {
    SceneManager::set_sprite_position(
        &uuid::Uuid::parse_str(scene_id).unwrap(),
        &uuid::Uuid::parse_str(sprite_id).unwrap(),
        x,
        y,
        z
    );
}