#version 330 core

layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal_modelspace;

out vec2 uv;
out vec3 position_worldspace;
out vec3 normal_camera_space;
out vec3 eye_direction_cameraspace;
out vec3 light_direction_cameraspace;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 light_position_worldspace;

void main()
{
  gl_Position = MVP * vec4(vertex_position_modelspace, 1);

  position_worldspace = (M * vec4(vertex_position_modelspace, 1))->xyz;
}
