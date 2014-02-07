
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <stack>
#include <glload/gl_3_3.h>
#include <glutil/glutil.h>
#include <GL/freeglut.h>
#include "../framework/framework.h"
#include "../framework/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ProgramData
{
  GLuint the_program;
  GLuint model_to_world_matrix_uniform;
  GLuint world_to_camera_matrix_uniform;
  GLuint camera_to_clip_matrix_uniform;
  GLuint base_color_uniform;
};

float g_z_near = 1.0f;
float g_z_far = 1000.0f;

ProgramData UniformColor;
ProgramData UniformColorTint;

ProgramData LoadProgram(const std::string& str_vertex_shader,
                        const std::string& str_fragment_shader)
{
  std::vector<GLuint> shader_list;

  shader_list.push_back(Framework::LoadShader(GL_VERTEX_SHADER,
                                              str_vertex_shader));
  shader_list.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER,
                                              str_fragment_shader));

  ProgramData data;
  data.the_program = Framework::CreateProgram(shader_list);
  data.model_to_world_matrix_uniform = glGetUniformLocation(
        data.the_program, "model_to_world_matrix");
  data.world_to_camera_matrix_uniform = glGetUniformLocation(
        data.the_program, "world_to_camera_matrix");
  data.camera_to_clip_matrix_uniform = glGetUniformLocation(
        data.the_program, "camera_to_clip_matrix");
  data.base_color_uniform = glGetUniformLocation(
        data.the_program, "base_color");

  return data;
}

void InitializeProgram()
{
  UniformColor = LoadProgram("PosOnlyWorldTransform.vert",
                             "ColorUniform.frag");
  UniformColorTint = LoadProgram("PosColorWorldTransform.vert",
                                 "ColorMultUniform.frag");
}

glm::mat4 CalcLookAtMatrix(const glm::vec3& camera_point,
                           const glm::vec3& look_point,
                           const glm::vec3& up_point)
{
  glm::vec3 look_direction = glm::normalize(look_point - camera_point);
  glm::vec3 up_direction = glm::normalize(up_point);

  glm::vec3 right_direction = glm::normalize(
        glm::cross(look_direction, up_direction));
  glm::vec3 perp_up_direction = glm::cross(right_direction, look_direction);

  glm::mat4 rotation_matrix(1.0f);
  rotation_matrix[0] = glm::vec4(right_direction, 0.0f);
  rotation_matrix[1] = glm::vec4(perp_up_direction, 0.0f);
  rotation_matrix[2] = glm::vec4(-look_direction, 0.0f);

  rotation_matrix = glm::transpose(rotation_matrix);

  glm::mat4 translation_matrix(1.0f);
  translation_matrix[3] = glm::vec4(-camera_point, 1.0f);

  return rotation_matrix * translation_matrix;
}

Framework::Mesh* g_cone_mesh = NULL;
Framework::Mesh* g_cylinder_mesh = NULL;
Framework::Mesh* g_cube_tint_mesh = NULL;
Framework::Mesh* g_cube_color_mesh = NULL;
Framework::Mesh* g_plane_mesh = NULL;

void init()
{
  InitializeProgram();

  try {
    g_cone_mesh = new Framework::Mesh("UnitConeTint.xml");
    g_cylinder_mesh = new Framework::Mesh("UnitCylinderTint.xml");
    g_cube_tint_mesh = new Framework::Mesh("UnitCubeTint.xml");
    g_cube_color_mesh = new Framework::Mesh("UnitCubeColor.xml");
    g_plane_mesh = new Framework::Mesh("UnitPlane.xml");
  }
  catch(std::exception& except){
    printf("%s\n", except.what());
    throw;
  }

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);
}

const float g_parthenon_width = 14.0f;
const float g_parthenon_length = 20.0f;
const float g_parthenon_column_height = 5.0f;
const float g_parthenon_base_height = 1.0f;
const float g_parthenon_top_height = 2.0f;

void DrawParthenon(glutil::MatrixStack& model_matrix)
{
  {
  // Draw base.

  glutil::PushStack push(model_matrix);
//  model_matrix.Push();

  model_matrix.Scale(glm::vec3(g_parthenon_width, g_parthenon_base_height,
                               g_parthenon_length));
  model_matrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

  glUseProgram(UniformColorTint.the_program);
  glUniformMatrix4fv(UniformColorTint.model_to_world_matrix_uniform, 1,
                     GL_FALSE, glm::value_ptr(model_matrix.Top()));
  glUniform4f(UniformColorTint.base_color_uniform, 0.9f, 0.9f, 0.9f, 0.9f);
  g_cube_tint_mesh->Render();
  glUseProgram(0);
  }

  {
  // Draw top.

  glutil::PushStack push(model_matrix);
//  model_matrix.Push();

  model_matrix.Translate(
        glm::vec3(0.0f,
                  g_parthenon_column_height + g_parthenon_base_height,
                  0.0f));
  model_matrix.Scale(glm::vec3(g_parthenon_width, g_parthenon_top_height,
                               g_parthenon_length));
  model_matrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

  glUseProgram(UniformColorTint.the_program);
  glUniformMatrix4fv(UniformColorTint.model_to_world_matrix_uniform, 1,
                     GL_FALSE, glm::value_ptr(model_matrix.Top()));
  glUniform4f(UniformColorTint.base_color_uniform, 0.9f, 0.9f, 0.9f, 0.9f);
  g_cube_tint_mesh->Render();
  glUseProgram(0);
  }
}

static glm::vec3 g_camera_target(0.0f, 0.4f, 0.0f);

static glm::vec3 g_sphere_camera_rel_position(67.5f, -46.0f, 150.0f);

glm::vec3 ResolveCameraPosition()
{
//  glutil::MatrixStack temp_matrix;

  float phi = Framework::DegToRad(g_sphere_camera_rel_position.x);
  float theta = Framework::DegToRad(g_sphere_camera_rel_position.y + 90.0f);

  float sin_theta = sinf(theta);
  float cos_theta = cosf(theta);
  float cos_phi = cosf(phi);
  float sin_phi = sinf(phi);

  glm::vec3 dir_to_camera(sin_theta * cos_phi, cos_theta, sin_theta * sin_phi);
  return (dir_to_camera * g_sphere_camera_rel_position.z) + g_camera_target;
}

void display()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (g_cone_mesh && g_cylinder_mesh && g_cube_tint_mesh &&
      g_cube_color_mesh && g_plane_mesh) {
    const glm::vec3& camera_pos = ResolveCameraPosition();

    glutil::MatrixStack camera_matrix;
    camera_matrix.SetMatrix(CalcLookAtMatrix(camera_pos, g_camera_target,
                                             glm::vec3(0.0f, 1.0f, 0.0f)));

    glUseProgram(UniformColor.the_program);
    glUniformMatrix4fv(UniformColor.world_to_camera_matrix_uniform, 1, GL_FALSE,
                       glm::value_ptr(camera_matrix.Top()));
    glUseProgram(UniformColorTint.the_program);
    glUniformMatrix4fv(UniformColorTint.world_to_camera_matrix_uniform, 1,
                       GL_FALSE, glm::value_ptr(camera_matrix.Top()));
    glUseProgram(0);

    glutil::MatrixStack model_matrix;

    //
    // Render the ground plane.
    //
    {
    glutil::PushStack push(model_matrix);
//    model_matrix.Push();

    model_matrix.Scale(glm::vec3(100.0f, 1.0f, 100.0f));

    glUseProgram(UniformColor.the_program);
    glUniformMatrix4fv(UniformColor.model_to_world_matrix_uniform, 1, GL_FALSE,
                       glm::value_ptr(model_matrix.Top()));
    glUniform4f(UniformColor.base_color_uniform, 0.302f, 0.416f, 0.0589f, 1.0f);
    g_plane_mesh->Render();
    glUseProgram(0);
    }

    //
    // Draw the building.
    //
    {
    glutil::PushStack push(model_matrix);
//    model_matrix.Push();
    model_matrix.Translate(glm::vec3(20.0f, 0.0f, -10.0f));

    DrawParthenon(model_matrix);
    }

  } // end if

  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glutil::MatrixStack perspective_matrix;
  perspective_matrix.Perspective(45.0f, (w / (float)h), g_z_near, g_z_far);

  glUseProgram(UniformColor.the_program);
  glUniformMatrix4fv(UniformColor.camera_to_clip_matrix_uniform, 1, GL_FALSE,
                     glm::value_ptr(perspective_matrix.Top()));
  glUseProgram(UniformColorTint.the_program);
  glUniformMatrix4fv(UniformColorTint.camera_to_clip_matrix_uniform, 1,
                     GL_FALSE, glm::value_ptr(perspective_matrix.Top()));
  glUseProgram(0);

  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 27:
    delete g_cone_mesh;
    g_cone_mesh = NULL;
    delete g_cube_tint_mesh;
    g_cube_tint_mesh = NULL;
    delete g_cube_color_mesh;
    g_cube_color_mesh = NULL;
    delete g_plane_mesh;
    g_plane_mesh = NULL;
    glutLeaveMainLoop();
    return;
  case 'w': g_camera_target.z -= 4.0f; break;
  case 's': g_camera_target.z += 4.0f; break;
  case 'd': g_camera_target.x += 4.0f; break;
  case 'a': g_camera_target.x -= 4.0f; break;
  case 'e': g_camera_target.y -= 4.0f; break;
  case 'q': g_camera_target.y += 4.0f; break;
  }

  glutPostRedisplay();
}

unsigned int defaults(unsigned int display_mode, int& width, int& height)
{
  return display_mode;
}
