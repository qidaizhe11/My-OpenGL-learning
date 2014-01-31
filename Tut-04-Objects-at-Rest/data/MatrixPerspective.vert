
#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

smooth out vec4 the_color;

uniform vec2 offset;
uniform float z_near;
uniform float z_far;
uniform float frustum_scale;

void main()
{
  vec4 camera_pos = position + vec4(offset.x, offset.y, 0.0f, 0.0f);
  vec4 clip_pos;

  clip_pos.xy = camera_pos.xy * frustum_scale;

  clip_pos.z = camera_pos.z * (z_near + z_far) / (z_near - z_far);
  clip_pos.z += 2 * z_near * z_far / (z_near - z_far);

  clip_pos.w = -camera_pos.z;

  gl_Position = clip_pos;
  the_color = color;
}
