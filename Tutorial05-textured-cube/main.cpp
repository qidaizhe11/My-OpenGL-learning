
#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../common/shader.hpp"
#include "../common/texture.h"

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

  if (!glfwOpenWindow(800, 600, 0, 0, 0, 0, 32, 0, GLFW_WINDOW)) {
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

  glfwSetWindowTitle("Tuturial 05");

  glfwEnable(GLFW_STICKY_KEYS);

  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  GLuint vertex_array_id;
  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  GLuint programID = LoadShaders("Transform.vert", "Texture.frag");
  GLuint matrixID = glGetUniformLocation(programID, "MVP");

  glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  glm::mat4 view = glm::lookAt(glm::vec3(4, 3, 3),
                               glm::vec3(0, 0, 0),
                               glm::vec3(0, 1, 0));
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 MVP = projection * view * model;

  GLuint texture = loadDDS("uvtemplate.DDS");

  GLuint texture_id = glGetUniformLocation(programID, "my_texture_sampler");

  static const GLfloat g_vertex_buffer_data[] = {
      -1.0f,-1.0f,-1.0f,
      -1.0f,-1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f,
       1.0f, 1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f,
       1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f,-1.0f,
       1.0f,-1.0f,-1.0f,
       1.0f, 1.0f,-1.0f,
       1.0f,-1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,
       1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f,-1.0f, 1.0f,
       1.0f,-1.0f, 1.0f,
       1.0f, 1.0f, 1.0f,
       1.0f,-1.0f,-1.0f,
       1.0f, 1.0f,-1.0f,
       1.0f,-1.0f,-1.0f,
       1.0f, 1.0f, 1.0f,
       1.0f,-1.0f, 1.0f,
       1.0f, 1.0f, 1.0f,
       1.0f, 1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f,
       1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
       1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f,
       1.0f,-1.0f, 1.0f
    };

  // Two UV coordinatesfor each vertex. They were created withe Blender.
    static const GLfloat g_uv_buffer_data[] = {
      0.000059f, 1.0f-0.000004f,
      0.000103f, 1.0f-0.336048f,
      0.335973f, 1.0f-0.335903f,
      1.000023f, 1.0f-0.000013f,
      0.667979f, 1.0f-0.335851f,
      0.999958f, 1.0f-0.336064f,
      0.667979f, 1.0f-0.335851f,
      0.336024f, 1.0f-0.671877f,
      0.667969f, 1.0f-0.671889f,
      1.000023f, 1.0f-0.000013f,
      0.668104f, 1.0f-0.000013f,
      0.667979f, 1.0f-0.335851f,
      0.000059f, 1.0f-0.000004f,
      0.335973f, 1.0f-0.335903f,
      0.336098f, 1.0f-0.000071f,
      0.667979f, 1.0f-0.335851f,
      0.335973f, 1.0f-0.335903f,
      0.336024f, 1.0f-0.671877f,
      1.000004f, 1.0f-0.671847f,
      0.999958f, 1.0f-0.336064f,
      0.667979f, 1.0f-0.335851f,
      0.668104f, 1.0f-0.000013f,
      0.335973f, 1.0f-0.335903f,
      0.667979f, 1.0f-0.335851f,
      0.335973f, 1.0f-0.335903f,
      0.668104f, 1.0f-0.000013f,
      0.336098f, 1.0f-0.000071f,
      0.000103f, 1.0f-0.336048f,
      0.000004f, 1.0f-0.671870f,
      0.336024f, 1.0f-0.671877f,
      0.000103f, 1.0f-0.336048f,
      0.336024f, 1.0f-0.671877f,
      0.335973f, 1.0f-0.335903f,
      0.667969f, 1.0f-0.671889f,
      1.000004f, 1.0f-0.671847f,
      0.667979f, 1.0f-0.335851f
    };

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
               g_vertex_buffer_data, GL_STATIC_DRAW);

  GLuint uvbuffer;
  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data),
               g_uv_buffer_data, GL_STATIC_DRAW);

  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    gluniform1i(texture_id, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 12*3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glfwSwapBuffers();
  } while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
           glfwGetWindowParam(GLFW_OPENED));

  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &uvbuffer);
  glDeleteProgram(programID);
  glDeleteVertexArrays(1, &vertex_array_id);

  glfwTerminate();
  return 0;
}
