
#include <cstdio>
#include <cstdlib>
#include <vector>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../common/shader.hpp"
#include "../common/texture.h"
#include "../common/controls.h"
#include "../common/objloader.h"

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

  glfwSetWindowTitle("Tuturial 07");

  glfwEnable(GLFW_STICKY_KEYS);
  glfwSetMousePos(800 / 2, 600 / 2);

  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glEnable(GL_CULL_FACE);

  GLuint vertex_array_id;
  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  GLuint programID = LoadShaders("Transform.vert", "Texture.frag");
  GLuint matrixID = glGetUniformLocation(programID, "MVP");

//  GLuint texture = loadTGA_glfw("uvtemplate.tga");
  GLuint texture = loadDDS("uvtemplate.DDS");

  GLuint texture_id = glGetUniformLocation(programID, "my_texture_sampler");

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;
  bool res = loadOBJ("cube.obj", vertices, uvs, normals);

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
               &vertices[0], GL_STATIC_DRAW);

  GLuint uvbuffer;
  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2),
               &uvs[0], GL_STATIC_DRAW);

  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    computeMatricesFromInputs();
    glm::mat4 projection_matrix = getProjectionMatrix();
    glm::mat4 view_matrix = getViewMatrix();
    glm::mat4 model_matrix = glm::mat4(1.0);
    glm::mat4 MVP = projection_matrix * view_matrix * model_matrix;

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(texture_id, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glfwSwapBuffers();
  } while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
           glfwGetWindowParam(GLFW_OPENED));

  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &uvbuffer);
  glDeleteProgram(programID);
  glDeleteTextures(1, &texture_id);
  glDeleteVertexArrays(1, &vertex_array_id);

  glfwTerminate();
  return 0;
}
