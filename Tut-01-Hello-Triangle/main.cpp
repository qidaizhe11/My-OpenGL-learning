#include <algorithm>
#include <string>
#include <vector>
#include <cstdio>
#include <glload/gl_3_2_comp.h>
#include <GL/freeglut.h>

GLuint CreateShader(GLenum e_shader_type, const std::string& str_shader_file)
{
  GLuint shader = glCreateShader(e_shader_type);
  const char* str_file_data = str_shader_file.c_str();
  glShaderSource(shader, 1, &str_file_data, NULL);

  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint info_log_length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

    GLchar* str_info_log = new GLchar[info_log_length + 1];
    glGetShaderInfoLog(shader,info_log_length, NULL, str_info_log);

    const char* str_shader_type = NULL;
    switch (e_shader_type) {
    case GL_VERTEX_SHADER: str_shader_type = "vertex"; break;
    case GL_GEOMETRY_SHADER: str_shader_type = "geometry"; break;
    case GL_FRAGMENT_SHADER: str_shader_type = "fragment"; break;
    }

    fprintf(stderr, "Compile failure in %s shader:\n%s\n", str_shader_type,
            str_info_log);
    delete [] str_info_log;
  }

  return shader;
}

GLuint CreateProgram(const std::vector<GLuint>& shader_list)
{
  GLuint program = glCreateProgram();

  for (size_t i = 0; i < shader_list.size(); ++i) {
    glAttachShader(program, shader_list[i]);
  }

  glLinkProgram(program);

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint info_log_length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

    GLchar* str_info_log = new GLchar[info_log_length + 1];
    glGetProgramInfoLog(program, info_log_length, NULL, str_info_log);
    fprintf(stderr, "Linker failure: %s\n", str_info_log);
    delete [] str_info_log;
  }

  for (size_t i = 0; i < shader_list.size(); ++i) {
    glDetachShader(program, shader_list[i]);
  }

  return program;
}

GLuint the_program;

const std::string str_vertex_shader(
    "#version 330\n"
    "layout(location = 0) in vec4 position;\n"
    "void main() {\n"
    "  gl_Position = position;\n"
    "}\n");

const std::string str_fragment_shader(
    "#version 330\n"
    "out vec4 output_color;\n"
    "void main() {\n"
    "  output_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n");

void InitializeProgram()
{
  std::vector<GLuint> shader_list;

  shader_list.push_back(CreateShader(GL_VERTEX_SHADER, str_vertex_shader));
  shader_list.push_back(CreateShader(GL_FRAGMENT_SHADER, str_fragment_shader));

  the_program = CreateProgram(shader_list);

  std::for_each(shader_list.begin(), shader_list.end(), glDeleteShader);
}

const float vertex_positions[] = {
  0.75f, 0.75f, 0.0f, 1.0f,
  0.75f, -0.75f, 0.0f, 1.0f,
  -0.75f, -0.75f, 0.0f, 1.0f,
};

GLuint position_buffer_object;
GLuint vao;

void InitializeVertexBuffer()
{
  glGenBuffers(1, &position_buffer_object);

  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void init()
{
  InitializeProgram();
  InitializeVertexBuffer();

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
}

void display()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(the_program);

  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_object);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(0);
  glUseProgram(0);

  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 27:  // escape key
    glutLeaveMainLoop();
    return;

    break;
  default:
    break;
  }
}

unsigned int defaults(unsigned int display_mode, int& width, int& height)
{
  return display_mode;
}
