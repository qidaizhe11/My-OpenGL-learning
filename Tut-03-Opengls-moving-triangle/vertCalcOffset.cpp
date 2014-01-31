#include <string>
#include <vector>
#include <math.h>
#include <cstdio>
#include <cstring>
#include <glload/gl_3_3.h>
#include <GL/freeglut.h>

#include "../framework/framework.h"

GLuint the_program;
GLuint elapsed_time_uniform;

void InitializeProgram()
{
  std::vector<GLuint> shader_list;

  shader_list.push_back(Framework::LoadShader(GL_VERTEX_SHADER,
                                              "calcOffset.vert"));
  shader_list.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER,
                                              "standard.frag"));

  the_program = Framework::CreateProgram(shader_list);

  elapsed_time_uniform = glGetUniformLocation(the_program, "time");

  GLuint loop_duration_uniform = glGetUniformLocation(the_program,
                                                      "loop_duration");

  glUseProgram(the_program);
  glUniform1f(loop_duration_uniform, 5.0f);
  glUseProgram(0);
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

void display()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(the_program);

  glUniform1f(elapsed_time_uniform, glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

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


