import api

def on_frame(scene_uuid, sprite_uuid, x, y, delta_time):
    # Code for Right Arrow Key
    if api.key_pressed(262):
        api.set_sprite_position(scene_uuid, sprite_uuid, x + 0.5 * delta_time, y)
    # Code for Left Arrow Key
    if api.key_pressed(263):
        api.set_sprite_position(scene_uuid, sprite_uuid, x - 0.5 * delta_time, y)