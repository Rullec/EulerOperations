#version 330 core
layout (location = 0) in vec3 Position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;

out vec4 Color;

void main()
{
    vec4 model_pos = model * vec4(Position.x, Position.y, Position.z, 1.0);
    gl_Position = projection * view * model_pos;
    Color = vec4(clamp(color, 0.0, 1.0));
}
