#include <string>
#include <vector>
#include <math.h>
#include <cstdio>
#include <cstring>
#include <glload/gl_3_3.h>
#include <GL/freeglut.h>

#include "../framework/framework.h"

GLuint the_program;
GLuint offset_location;

void InitializeProgram()
{
  std::vector<GLuint> shader_list;

  shader_list.push_back(Framework::LoadShader(GL_VERTEX_SHADER,
                                              "positionOffset.vert"));
  shader_list.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER,
                                              "standard.frag"));

  the_program = Framework::CreateProgram(shader_list);

  offset_location = glGetUniformLocation(the_program, "offset");
}

const float vertex_positions[] = {
  0.25f, 0.25f, 0.0f, 1.0f,
  0.25f, -0.25f, 0.0f, 1.0f,
  -0.25f, -0.25f, 0.0f, 1.0f,
};

GLuint position_buffer_object;
GLuint vao;

void InitializeVeretxBuffer()
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
  InitializeVeretxBuffer();

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
}

void ComputePositionOffsets(float& x_offset, float& y_offset)
{
  const float loop_duration = 5.0f;
  const float scale = 3.14159f * 2.0f / loop_duration;

  float elapsed_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
  float curr_time_through_loop = fmodf(elapsed_time, loop_duration);

  x_offset = cosf(curr_time_through_loop * scale) * 0.5f;
  y_offset = sinf(curr_time_through_loop * scale) * 0.5f;
}

void display()
{
  float x_offset = 0.0f, y_offset = 0.0f;
  ComputePositionOffsets(x_offset, y_offset);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(the_program);

  glUniform2f(offset_location, x_offset, y_offset);

  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_object);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(0);
  glUseProgram(0);

  glutSwapBuffers();
  glutPostRedisplay();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 27:
    glutLeaveMainLoop();
    return;
  }
}

unsigned int defaults(unsigned int display_mode, int& width, int& height)
{
  return display_mode;
}


