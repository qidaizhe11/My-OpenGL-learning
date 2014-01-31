#version 330

out vec4 output_color;

void main()
{
  float lerp_value = gl_FragCoord.y / 500.0f;

  output_color = mix( vec4(1.0f, 1.0f, 1.0f, 1.0f),
                      vec4(0.2f, 0.2f, 0.2f, 1.0f),
                      lerp_value);
}
