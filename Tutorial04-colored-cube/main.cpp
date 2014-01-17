
#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../common/shader.hpp"

using namespace glm;

int main()
{
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }

  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (!glfwOpenWindow(960, 600, 0, 0, 0, 0, 32, 0, GLFW_WINDOW)) {
    fprintf(stderr, "Falied to open GLFW window. If you have an Intel GPU, "
            "they are not 3.3 compatible. "
            "Try the 2.1 version of the tutorials\n");
    glfwTerminate();
    return -1;
  }

  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW.\n");
    return -1;
  }

  glfwSetWindowTitle("Tuturial 03");

  glfwEnable(GLFW_STICKY_KEYS);

  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  GLuint vertex_array_id;
  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  GLuint programID = LoadShaders("SimpleTransform.vert", "SingleColor.frag");
  GLuint matrixID = glGetUniformLocation(programID, "MVP");

  glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  glm::mat4 view = glm::lookAt(glm::vec3(4, 3, 3),
                               glm::vec3(0, 0, 0),
                               glm::vec3(0, 1, 0));
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 MVP = projection * view * model;

  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,
  };
//  static const GLushort g_element_buffer_data[] = {0, 1, 2};

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
               g_vertex_buffer_data, GL_STATIC_DRAW);

  do {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(programID);

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    glfwSwapBuffers();
  } while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
           glfwGetWindowParam(GLFW_OPENED));

  glDeleteBuffers(1, &vertexbuffer);
  glDeleteProgram(programID);
  glDeleteVertexArrays(1, &vertex_array_id);

  glfwTerminate();
  return 0;
}
