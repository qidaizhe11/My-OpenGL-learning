#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D my_texture_sampler;

void main() {
  color = texture2D(my_texture_sampler, UV).rgb;
}
