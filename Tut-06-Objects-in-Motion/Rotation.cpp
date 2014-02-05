
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <glload/gl_3_3.h>
#include <GL/freeglut.h>
#include "../framework/framework.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint the_program;

GLuint model_to_camera_matrix_uniform;
GLuint camera_to_clip_matrix_uniform;

glm::mat4 camera_to_clip_matrix(0.0f);

float CalcFrustumScale(float fov_deg)
{
  const float deg_to_rad = 3.14159f * 2.0f / 360.0f;
  float fov_rad = fov_deg * deg_to_rad;
  return 1.0f / tan(fov_rad / 2.0f);
}

const float frustum_scale = CalcFrustumScale(45.0f);

void InitializeProgram()
{
  std::vector<GLuint> shader_list;

  shader_list.push_back(Framework::LoadShader(GL_VERTEX_SHADER,
                                              "PosColorLocalTransform.vert"));
  shader_list.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER,
                                              "ColorPassthrough.frag"));

  the_program = Framework::CreateProgram(shader_list);

  model_to_camera_matrix_uniform = glGetUniformLocation(
        the_program, "model_to_camera_matrix");
  camera_to_clip_matrix_uniform = glGetUniformLocation(
        the_program, "camera_to_clip_matrix");

  float z_near = 1.0f;
  float z_far = 61.0f;

  camera_to_clip_matrix[0].x = frustum_scale;
  camera_to_clip_matrix[1].y = frustum_scale;
  camera_to_clip_matrix[2].z = (z_far + z_near) / (z_near - z_far);
  camera_to_clip_matrix[2].w = -1.0f;
  camera_to_clip_matrix[3].z = (2 * z_far * z_near) / (z_near - z_far);

  glUseProgram(the_program);
  glUniformMatrix4fv(camera_to_clip_matrix_uniform, 1, GL_FALSE,
                     glm::value_ptr(camera_to_clip_matrix));
  glUseProgram(0);
}

const int number_of_vertices = 8;

#define GREEN_COLOR 0.0f, 1.0f, 0.0f, 1.0f
#define BLUE_COLOR 0.0f, 0.0f, 1.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR 0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f

const float vertex_data[] = {
  1.0f, 1.0f, 1.0f,
  -1.0f, -1.0f, 1.0f,
  -1.0f, 1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,

  -1.0f, -1.0f, -1.0f,
  1.0f, 1.0f, -1.0f,
  1.0f, -1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,

  GREEN_COLOR,
  BLUE_COLOR,
  RED_COLOR,
  BROWN_COLOR,

  GREEN_COLOR,
  BLUE_COLOR,
  RED_COLOR,
  BROWN_COLOR,
};

const GLshort index_data[] = {
  0, 1, 2,
  1, 0, 3,
  2, 3, 0,
  3, 2, 1,

  5, 4, 6,
  4, 5, 7,
  7, 6, 4,
  6, 7, 5,
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

float CalcLerpFactor(float elapsed_time, float loop_duration)
{
  float value = fmodf(elapsed_time, loop_duration) / loop_duration;
  if (value > 0.5f) {
    value = 1.0f - value;
  }

  return value * 2.0f;
}

glm::vec3 NullScale(float elapsed_time)
{
  return glm::vec3(1.0f, 1.0f, 1.0f);
}

glm::vec3 StaticUniformScale(float elapsed_time)
{
  return glm::vec3(4.0f, 4.0f, 4.0f);
}

glm::vec3 StaticNonUniformScale(float elapsed_time)
{
  return glm::vec3(0.5f, 1.0f, 10.0f);
}

glm::vec3 DynamicUniformScale(float elapsed_time)
{
  const float loop_duration = 3.0f;

  return glm::vec3(glm::mix(1.0f, 4.0f,
                            CalcLerpFactor(elapsed_time, loop_duration)));
}

glm::vec3 DynamicNonUniformScale(float elapsed_time)
{
  const float x_loop_duration = 3.0f;
  const float z_loop_duration = 5.0f;

  return glm::vec3(glm::mix(1.0f, 0.5f,
                            CalcLerpFactor(elapsed_time, x_loop_duration)),
                   1.0f,
                   glm::mix(1.0f, 10.0f,
                            CalcLerpFactor(elapsed_time, z_loop_duration)));
}

struct Instance
{
  typedef glm::vec3(*ScaleFunc)(float);

  ScaleFunc CalcScale;
  glm::vec3 offset;

  glm::mat4 ConstructMatrix(float elapsed_time)
  {
    glm::vec3 the_scale = CalcScale(elapsed_time);
    glm::mat4 the_mat(1.0f);
    the_mat[0].x = the_scale.x;
    the_mat[1].y = the_scale.y;
    the_mat[2].z = the_scale.z;
    the_mat[3] = glm::vec4(offset, 1.0f);

    return the_mat;
  }
};

Instance g_instance_list[] = {
  {NullScale,               glm::vec3(0.0f, 0.0f, -45.0f)},
  {StaticUniformScale,      glm::vec3(-10.0f, -10.0f, -45.0f)},
  {StaticNonUniformScale,   glm::vec3(-10.0f, 10.0f, -45.0f)},
  {DynamicUniformScale,     glm::vec3(10.0f, 10.0f, -45.0f)},
  {DynamicNonUniformScale,  glm::vec3(10.0f, -10.0f, -45.0f)},
};

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

  float elapsed_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
  for(int i = 0; i < ARRAY_COUNT(g_instance_list); ++i) {
    Instance& curr_instance = g_instance_list[i];

    const glm::mat4& transform_matrix =
        curr_instance.ConstructMatrix(elapsed_time);
    glUniformMatrix4fv(model_to_camera_matrix_uniform, 1, GL_FALSE,
                       glm::value_ptr(transform_matrix));
    glDrawElements(GL_TRIANGLES, ARRAY_COUNT(index_data), GL_UNSIGNED_SHORT, 0);
  }

  glBindVertexArray(0);
  glUseProgram(0);

  glutSwapBuffers();
  glutPostRedisplay();
}

void reshape(int w, int h)
{
  camera_to_clip_matrix[0].x = frustum_scale * (h / (float)w);
  camera_to_clip_matrix[1].y = frustum_scale;

  glUseProgram(the_program);
  glUniformMatrix4fv(camera_to_clip_matrix_uniform, 1, GL_FALSE,
                     glm::value_ptr(camera_to_clip_matrix));

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



