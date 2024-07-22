use bevy_ecs::prelude::*;
use bevy_ecs::world::World;
use uuid::Uuid;
use web_sys::WebGl2RenderingContext;
extern crate nalgebra_glm as glm;





#[derive(Component)]
pub struct ID { pub id: uuid::Uuid, name: String }

#[derive(Component)]
pub struct Sprite { pub scale: glm::Vec2 }

#[derive(Component, Default)]
pub struct Transform { pub position: glm::Vec3, pub scale: glm::Vec2, pub rotation: f32 }


pub struct Scene {
    pub id: uuid::Uuid,
    pub name: String,
    pub world: World
}

impl Scene {
    pub fn add_entity(&mut self, name: String) -> Entity {
        self.world.spawn( ID { id: uuid::Uuid::new_v4(), name }).id()
    }

    pub fn add_sprite(&mut self, entity: Entity) {
        let mut entity_ref = self.world.entity_mut(entity);
        entity_ref.insert(Sprite { scale: glm::Vec2::new(0.1, 0.1) });
    }

    pub fn add_transform(&mut self, entity: Entity) {
        let mut entity_ref = self.world.entity_mut(entity);
        entity_ref.insert(Transform::default());
    }
}


#[derive(Default)]
pub struct SceneManager {
    pub scenes: std::collections::HashMap<uuid::Uuid, Scene>
}

impl SceneManager {
    pub fn get_scene(&mut self, id: &uuid::Uuid) -> Option<&mut Scene> {
        self.scenes.get_mut(id)
    }

    pub fn add_scene(&mut self, name: String) -> uuid::Uuid {
        let id = Uuid::new_v4();
        self.scenes.insert(id, Scene { id, name, world: World::default() });
        id
    }
}
