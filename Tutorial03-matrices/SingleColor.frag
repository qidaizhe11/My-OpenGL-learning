//uniform sampler2D qt_Texture0;
//varying vec4 qt_TexCoord0;

//void main(void)
//{
//    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
//}

#version 330 core

out vec3 color;

void main()
{
  color = vec3(1, 0, 0);
}
