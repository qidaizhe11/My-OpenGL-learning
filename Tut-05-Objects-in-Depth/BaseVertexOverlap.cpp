
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
const float frustum_scale = 1.0f;

void InitializeProgram()
{
  std::vector<GLuint> shader_list;

  shader_list.push_back(Framework::LoadShader(GL_VERTEX_SHADER,
                                              "Standard.vert"));
  shader_list.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER,
                                              "Standard.frag"));

  the_program = Framework::CreateProgram(shader_list);

  offset_uniform = glGetUniformLocation(the_program, "offset");

  perspective_matrix_uniform = glGetUniformLocation(the_program,
                                                    "perspective_matrix");

  float z_near = 1.0f;
  float z_far = 3.0f;

  memset(perspective_matrix, 0, sizeof(float) * 16);
  perspective_matrix[0] = frustum_scale;
  perspective_matrix[5] = frustum_scale;
  perspective_matrix[10] = (z_far + z_near) / (z_near - z_far);
  perspective_matrix[14] = (2 * z_far * z_near) / (z_near - z_far);
  perspective_matrix[11] = -1.0f;

  glUseProgram(the_program);
  glUniformMatrix4fv(perspective_matrix_uniform, 1, GL_FALSE,
                     perspective_matrix);
  glUseProgram(0);
}

const int number_of_vertices = 36;

#define RIGHT_EXTENT 0.8f
#define LEFT_EXTENT -RIGHT_EXTENT
#define TOP_EXTENT 0.20f
#define MIDDLE_EXTENT 0.0f
#define BOTTOM_EXTENT -TOP_EXTENT
#define FRONT_EXTENT -1.25f
#define REAR_EXTENT -1.75f

#define GREEN_COLOR 0.75f, 0.75f, 1.0f, 1.0f
#define BLUE_COLOR 0.0f, 0.5f, 0.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR 0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f

const float vertex_data[] = {
  //Object 1 positions
    LEFT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
    LEFT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
    RIGHT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
    RIGHT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,

    LEFT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,
    LEFT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
    RIGHT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
    RIGHT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,

    LEFT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
    LEFT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
    LEFT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,

    RIGHT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
    RIGHT_EXTENT,	MIDDLE_EXTENT,	FRONT_EXTENT,
    RIGHT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,

    LEFT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,
    LEFT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
    RIGHT_EXTENT,	TOP_EXTENT,		REAR_EXTENT,
    RIGHT_EXTENT,	BOTTOM_EXTENT,	REAR_EXTENT,

  //	0, 2, 1,
  //	3, 2, 0,

    //Object 2 positions
    TOP_EXTENT,		RIGHT_EXTENT,	REAR_EXTENT,
    MIDDLE_EXTENT,	RIGHT_EXTENT,	FRONT_EXTENT,
    MIDDLE_EXTENT,	LEFT_EXTENT,	FRONT_EXTENT,
    TOP_EXTENT,		LEFT_EXTENT,	REAR_EXTENT,

    BOTTOM_EXTENT,	RIGHT_EXTENT,	REAR_EXTENT,
    MIDDLE_EXTENT,	RIGHT_EXTENT,	FRONT_EXTENT,
    MIDDLE_EXTENT,	LEFT_EXTENT,	FRONT_EXTENT,
    BOTTOM_EXTENT,	LEFT_EXTENT,	REAR_EXTENT,

    TOP_EXTENT,		RIGHT_EXTENT,	REAR_EXTENT,
    MIDDLE_EXTENT,	RIGHT_EXTENT,	FRONT_EXTENT,
    BOTTOM_EXTENT,	RIGHT_EXTENT,	REAR_EXTENT,

    TOP_EXTENT,		LEFT_EXTENT,	REAR_EXTENT,
    MIDDLE_EXTENT,	LEFT_EXTENT,	FRONT_EXTENT,
    BOTTOM_EXTENT,	LEFT_EXTENT,	REAR_EXTENT,

    BOTTOM_EXTENT,	RIGHT_EXTENT,	REAR_EXTENT,
    TOP_EXTENT,		RIGHT_EXTENT,	REAR_EXTENT,
    TOP_EXTENT,		LEFT_EXTENT,	REAR_EXTENT,
    BOTTOM_EXTENT,	LEFT_EXTENT,	REAR_EXTENT,

    //Object 1 colors
    GREEN_COLOR,
    GREEN_COLOR,
    GREEN_COLOR,
    GREEN_COLOR,

    BLUE_COLOR,
    BLUE_COLOR,
    BLUE_COLOR,
    BLUE_COLOR,

    RED_COLOR,
    RED_COLOR,
    RED_COLOR,

    GREY_COLOR,
    GREY_COLOR,
    GREY_COLOR,

    BROWN_COLOR,
    BROWN_COLOR,
    BROWN_COLOR,
    BROWN_COLOR,

    //Object 2 colors
    RED_COLOR,
    RED_COLOR,
    RED_COLOR,
    RED_COLOR,

    BROWN_COLOR,
    BROWN_COLOR,
    BROWN_COLOR,
    BROWN_COLOR,

    BLUE_COLOR,
    BLUE_COLOR,
    BLUE_COLOR,

    GREEN_COLOR,
    GREEN_COLOR,
    GREEN_COLOR,

    GREY_COLOR,
    GREY_COLOR,
    GREY_COLOR,
    GREY_COLOR,
};

const GLshort index_data[] = {
  0, 2, 1,
  3, 2, 0,

  4, 5, 6,
  6, 7, 4,

  8, 9, 10,
  11, 13, 12,

  14, 16, 15,
  17, 16, 14,
};

GLuint vertex_buffer_object;
GLuint index_buffer_object;
GLuint vao;

void InitializeVertexBuffer()
{
  glGenBuffers(1, &vertex_buffer_object);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &index_buffer_object);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void init()
{
  InitializeProgram();
  InitializeVertexBuffer();

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  size_t color_data_offset = sizeof(float) * 3 * number_of_vertices;
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)color_data_offset);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);

  glBindVertexArray(0);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);
}

void display()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(the_program);

  glBindVertexArray(vao);

  glUniform3f(offset_uniform, 0.0f, 0.0f, 0.0f);
  glDrawElements(GL_TRIANGLES, ARRAY_COUNT(index_data), GL_UNSIGNED_SHORT, 0);

  glUniform3f(offset_uniform, 0.0, 0.0f, -1.0f);
  glDrawElementsBaseVertex(GL_TRIANGLES, ARRAY_COUNT(index_data),
                           GL_UNSIGNED_SHORT, 0, number_of_vertices / 2);

  glBindVertexArray(0);
  glUseProgram(0);

  glutSwapBuffers();
}

void reshape(int w, int h)
{
  perspective_matrix[0] = frustum_scale * (h / (float)w);
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

