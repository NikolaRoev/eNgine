use std::{borrow::Borrow, collections::HashMap, sync::{Mutex, OnceLock}};

use uuid::Uuid;
use web_sys::{WebGl2RenderingContext, WebGlVertexArrayObject};
extern crate nalgebra_glm as glm;
use std::cell::RefCell;
use std::rc::Rc;




#[derive(Clone)]
pub struct ID { pub id: uuid::Uuid, pub name: String }

#[derive(Clone)]
pub struct Sprite { pub scale: glm::Vec2 }

#[derive(Default, Clone)]
pub struct Transform { pub position: glm::Vec3, pub scale: glm::Vec2, pub rotation: f32 }


#[derive(Default, Clone)]
pub struct Scene {
    pub name: String,
    
    pub entities: HashMap<uuid::Uuid, ID>,
    pub sprites: HashMap<uuid::Uuid, Sprite>,
    pub transforms: HashMap<uuid::Uuid, Transform>,
}


#[derive(Default)]
pub struct SceneManager {
    pub scenes: HashMap<String, Scene>
}

impl SceneManager {
    pub fn init() {
        let _ = SCENE_MANAGER.with(|e| {
            e.replace(Some(SceneManager::default()))
        });
    }


    pub fn get_scenes() -> Vec<Scene> {
        SCENE_MANAGER.with(|e| {
            let borrow = e.borrow();
            let scene_manager = borrow.as_ref().unwrap();

            scene_manager.scenes.clone().into_values().collect()
        })
    }

    pub fn add_scene(name: &String) -> bool {
        SCENE_MANAGER.with(|e| {
            let mut borrow = e.borrow_mut();
            let scene_manager = borrow.as_mut().unwrap();

            if !scene_manager.scenes.contains_key(name) {
                scene_manager.scenes.insert(name.clone(), Scene { name: name.clone(), ..Default::default() });
                return true;
            }
            
            false
        })
    }


    pub fn add_entity(scene_name: &String, entity_name: &str) -> Option<uuid::Uuid> {
        SCENE_MANAGER.with(|e| {
            let mut borrow = e.borrow_mut();
            let scene_manager = borrow.as_mut().unwrap();

            let id = uuid::Uuid::new_v4();
            if let Some(scene) = scene_manager.scenes.get_mut(scene_name) {
                scene.entities.insert(id, ID { id, name: entity_name.to_string() });
                return Some(id);
            }

            None
        })
    }

    pub fn add_sprite(scene_name: &String, entity: &uuid::Uuid) -> bool {
        SCENE_MANAGER.with(|e| {
            let mut borrow = e.borrow_mut();
            let scene_manager = borrow.as_mut().unwrap();

            if let Some(scene) = scene_manager.scenes.get_mut(scene_name) {
                scene.sprites.insert(*entity, Sprite { scale: glm::Vec2::new(0.1, 0.1) });

                return true;
            }

            false
        })
    }

    pub fn add_transform(scene_name: &String, entity: &uuid::Uuid) -> bool {
        SCENE_MANAGER.with(|e| {
            let mut borrow = e.borrow_mut();
            let scene_manager = borrow.as_mut().unwrap();

            if let Some(scene) = scene_manager.scenes.get_mut(scene_name) {
                scene.transforms.insert(*entity, Transform::default());

                return true;
            }

            false
        })
    }
}



thread_local! {
    pub static SCENE_MANAGER: RefCell<Option<SceneManager>> = const { RefCell::new(None) };
}
