#include <string>
#include <vector>
#include <math.h>
#include <cstdio>
#include <cstring>
#include <glload/gl_3_3.h>
#include <GL/freeglut.h>

#include "../framework/framework.h"

//#define ARRAY_COUNT(array) (sizeof(array) / (sizeof(array[0]) * (sizeof(array) != sizeof(void*) ||  sizeof(array[0]) <= sizeof(void*))))

GLuint the_program;
GLuint elapsed_time_uniform;

void InitializeProgram()
{
  std::vector<GLuint> shader_list;

  shader_list.push_back(Framework::LoadShader(GL_VERTEX_SHADER,
                                              "standard.vert"));
  shader_list.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER,
                                              "standard.frag"));

  the_program = Framework::CreateProgram(shader_list);
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

void AdjustVertexData(float x_offset, float y_offset)
{
  std::vector<float> new_data(ARRAY_COUNT(vertex_positions));
  memcpy(&new_data[0], vertex_positions, sizeof(vertex_positions));

  for(int i = 0; i < ARRAY_COUNT(vertex_positions); i += 4) {
    new_data[i] += x_offset;
    new_data[i + 1] += y_offset;
  }

  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_object);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), &new_data[0]);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display()
{
  float x_offset = 0.0f, y_offset = 0.0f;
  ComputePositionOffsets(x_offset, y_offset);
  AdjustVertexData(x_offset, y_offset);

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

