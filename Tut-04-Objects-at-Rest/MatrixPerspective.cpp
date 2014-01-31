
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <glload/gl_3_3.h>
#include <GL/freeglut.h>
#include "../framework/framework.h"

GLuint the_program;

GLuint offset_uniform;

GLuint perspective_matrix_uniform;

float perspective_matrix[16];
const float frustum_scale =1.0f;

void InitializeProgram()
{
  std::vector<GLuint> shader_list;

  shader_list.push_back(Framework::LoadShader(GL_VERTEX_SHADER,
                                              "MatrixPerspective.vert"));
  shader_list.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER,
                                              "StandardColors.frag"));

  the_program = Framework::CreateProgram(shader_list);

  offset_uniform = glGetUniformLocation(the_program, "offset");

  perspective_matrix_uniform = glGetUniformLocation(the_program,
                                                    "perspective_matrix");

  float z_near = 0.5f;
  float z_far = 3.0f;

  memset(perspective_matrix, 0, sizeof(float) * 16);

  perspective_matrix[0] = frustum_scale;
  perspective_matrix[5] = frustum_scale;
  perspective_matrix[10] = (z_far + z_near) / (z_near - z_far);
  perspective_matrix[14] = (2 * z_far * z_near) / (z_near - z_far);
  perspective_matrix[11] = -1.0f;

  glUseProgram(the_program);
  glUniformMatrix4fv(perspective_matrix_uniform, 1, GL_FALSE, perspective_matrix);
  glUseProgram(0);
}

const float vertex_data[] = {
  0.25f,  0.25f, -1.25f, 1.0f,
    0.25f, -0.25f, -1.25f, 1.0f,
   -0.25f,  0.25f, -1.25f, 1.0f,

    0.25f, -0.25f, -1.25f, 1.0f,
   -0.25f, -0.25f, -1.25f, 1.0f,
   -0.25f,  0.25f, -1.25f, 1.0f,

    0.25f,  0.25f, -2.75f, 1.0f,
   -0.25f,  0.25f, -2.75f, 1.0f,
    0.25f, -0.25f, -2.75f, 1.0f,

    0.25f, -0.25f, -2.75f, 1.0f,
   -0.25f,  0.25f, -2.75f, 1.0f,
   -0.25f, -0.25f, -2.75f, 1.0f,

   -0.25f,  0.25f, -1.25f, 1.0f,
   -0.25f, -0.25f, -1.25f, 1.0f,
   -0.25f, -0.25f, -2.75f, 1.0f,

   -0.25f,  0.25f, -1.25f, 1.0f,
   -0.25f, -0.25f, -2.75f, 1.0f,
   -0.25f,  0.25f, -2.75f, 1.0f,

    0.25f,  0.25f, -1.25f, 1.0f,
    0.25f, -0.25f, -2.75f, 1.0f,
    0.25f, -0.25f, -1.25f, 1.0f,

    0.25f,  0.25f, -1.25f, 1.0f,
    0.25f,  0.25f, -2.75f, 1.0f,
    0.25f, -0.25f, -2.75f, 1.0f,

    0.25f,  0.25f, -2.75f, 1.0f,
    0.25f,  0.25f, -1.25f, 1.0f,
   -0.25f,  0.25f, -1.25f, 1.0f,

    0.25f,  0.25f, -2.75f, 1.0f,
   -0.25f,  0.25f, -1.25f, 1.0f,
   -0.25f,  0.25f, -2.75f, 1.0f,

    0.25f, -0.25f, -2.75f, 1.0f,
   -0.25f, -0.25f, -1.25f, 1.0f,
    0.25f, -0.25f, -1.25f, 1.0f,

    0.25f, -0.25f, -2.75f, 1.0f,
   -0.25f, -0.25f, -2.75f, 1.0f,
   -0.25f, -0.25f, -1.25f, 1.0f,




   0.0f, 0.0f, 1.0f, 1.0f,
   0.0f, 0.0f, 1.0f, 1.0f,
   0.0f, 0.0f, 1.0f, 1.0f,

   0.0f, 0.0f, 1.0f, 1.0f,
   0.0f, 0.0f, 1.0f, 1.0f,
   0.0f, 0.0f, 1.0f, 1.0f,

   0.8f, 0.8f, 0.8f, 1.0f,
   0.8f, 0.8f, 0.8f, 1.0f,
   0.8f, 0.8f, 0.8f, 1.0f,

   0.8f, 0.8f, 0.8f, 1.0f,
   0.8f, 0.8f, 0.8f, 1.0f,
   0.8f, 0.8f, 0.8f, 1.0f,

   0.0f, 1.0f, 0.0f, 1.0f,
   0.0f, 1.0f, 0.0f, 1.0f,
   0.0f, 1.0f, 0.0f, 1.0f,

   0.0f, 1.0f, 0.0f, 1.0f,
   0.0f, 1.0f, 0.0f, 1.0f,
   0.0f, 1.0f, 0.0f, 1.0f,

   0.5f, 0.5f, 0.0f, 1.0f,
   0.5f, 0.5f, 0.0f, 1.0f,
   0.5f, 0.5f, 0.0f, 1.0f,

   0.5f, 0.5f, 0.0f, 1.0f,
   0.5f, 0.5f, 0.0f, 1.0f,
   0.5f, 0.5f, 0.0f, 1.0f,

   1.0f, 0.0f, 0.0f, 1.0f,
   1.0f, 0.0f, 0.0f, 1.0f,
   1.0f, 0.0f, 0.0f, 1.0f,

   1.0f, 0.0f, 0.0f, 1.0f,
   1.0f, 0.0f, 0.0f, 1.0f,
   1.0f, 0.0f, 0.0f, 1.0f,

   0.0f, 1.0f, 1.0f, 1.0f,
   0.0f, 1.0f, 1.0f, 1.0f,
   0.0f, 1.0f, 1.0f, 1.0f,

   0.0f, 1.0f, 1.0f, 1.0f,
   0.0f, 1.0f, 1.0f, 1.0f,
   0.0f, 1.0f, 1.0f, 1.0f,
};

GLuint vertex_buffer_object;
GLuint vao;

void InitializeVertexBuffer()
{
  glGenBuffers(1, &vertex_buffer_object);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init()
{
  InitializeProgram();
  InitializeVertexBuffer();

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);
}

void display()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(the_program);

  glUniform2f(offset_uniform, 1.5f, 0.5f);

  size_t color_data = sizeof(vertex_data) / 2;
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)color_data);

  glDrawArrays(GL_TRIANGLES, 0, 36);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glUseProgram(0);

  glutSwapBuffers();
  glutPostRedisplay();
}

void reshape(int w, int h)
{
  perspective_matrix[0] = frustum_scale / (w / (float)h);
  perspective_matrix[5] = frustum_scale;

  glUseProgram(the_program);
  glUniformMatrix4fv(perspective_matrix_uniform, 1, GL_FALSE,
                     perspective_matrix);
  glUseProgram(0);

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
