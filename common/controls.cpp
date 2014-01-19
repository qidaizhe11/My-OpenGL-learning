
#include "controls.h"
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

glm::mat4 view_matrix;
glm::mat4 projection_matrix;

glm::mat4 getViewMatrix()
{
  return view_matrix;
}

glm::mat4 getProjectionMatrix()
{
  return projection_matrix;
}

glm::vec3 position = glm::vec3(0, 0, 5);
float horizontal_angle = 3.14f;
float vertical_angle = 0.0f;
float initial_fov = 45.0f;

float speed = 3.0f;
float mouse_speed = 0.005f;

void computeMatricesFromInputs()
{
  static double last_time = glfwGetTime();

  double current_time = glfwGetTime();
  float delta_time = float(current_time - last_time);

  int xpos, ypos;
  glfwGetMousePos(&xpos, &ypos);

  glfwSetMousePos(800/2, 600/2);

  horizontal_angle += mouse_speed * float(800/2 - xpos);
  vertical_angle += mouse_speed * float(600/2 - ypos);

  glm::vec3 direction(cos(vertical_angle) * sin(horizontal_angle),
                      sin(vertical_angle),
                      cos(vertical_angle) * cos(horizontal_angle));

  glm::vec3 right = glm::vec3(sin(horizontal_angle - 3.14/2.0f),
                              0,
                              cos(horizontal_angle - 3.14/2.0f));

  glm::vec3 up = glm::cross(right, direction);

  if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
    position += direction * delta_time * speed;
  }
  if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
    position -= direction * delta_time * speed;
  }
  if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
    position += right * delta_time * speed;
  }
  if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
    position -= right * delta_time * speed;
  }

  float fov = initial_fov - 5 * glfwGetMouseWheel();

  projection_matrix = glm::perspective(fov, 4.0f / 3.0f, 0.1f, 100.0f);
  view_matrix = glm::lookAt(position, position + direction, up);

  last_time = current_time;
}
