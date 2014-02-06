#version 330

layout(location = 0) in vec4 position;

uniform mat4 camera_to_clip_matrix;
uniform mat4 world_to_camera_matrix;
uniform mat4 model_to_world_matrix;

void main()
{
  vec4 temp = model_to_world_matrix * position;
  temp = world_to_camera_matrix * temp;
  gl_Position = camera_to_clip_matrix * temp;
}
