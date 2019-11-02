import json

total_conf = {}
main_scene_conf = {
    "StartX" : 100,
    "StartY" : 100,
    "Width" : 800,
    "Height" : 600,
    "ClearColor" : [0.4, 0.8, 1.0, 0.2]
}
render_conf = {
    "vertex_shader_path" : "../src/shaders/vertex.glsl",
    "fragment_shader_path" : "../src/shaders/fragment.glsl"
}
total_conf = {
    "MainWindowInfo" : main_scene_conf,
    "Render" : render_conf
}
with open("test.conf", "r+") as f:
    json.dump(total_conf, f, indent = 4)   