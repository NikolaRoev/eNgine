import api

move_right = False

def on_frame(scene_uuid, sprite_uuid, x, y, delta_time):
    global move_right

    if move_right:
        new_x = x + 0.5 * delta_time
        api.set_sprite_position(scene_uuid, sprite_uuid, new_x, y)
        if new_x > 500:
            move_right = False
    else:
        new_x = x - 0.5 * delta_time
        api.set_sprite_position(scene_uuid, sprite_uuid, new_x, y)
        if new_x < 100:
            move_right = True