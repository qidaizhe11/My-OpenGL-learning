#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <GL/glew.h>
#include <GL/glfw.h>

#include "texture.h"

GLuint loadTGA_glfw(const char *imagepath)
{
  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  glfwLoadTexture2D(imagepath, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);

  return texture_id;
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char *imagepath)
{
  unsigned char header[124];

  FILE* fp;

  fp = fopen(imagepath, "rb");
  if (fp == NULL) {
    printf("%s could not be opened. Are you in the right directory? Don't "
           "forget to read the FAQ!\n", imagepath);
    getchar();
    return 0;
  }

  char file_code[4];
  fread(file_code, 1, 4, fp);
  if (strncmp(file_code, "DDS ", 4) != 0) {
    fclose(fp);
    return 0;
  }

  fread(&header, 124, 1, fp);

  unsigned int height = *(unsigned int*)&(header[8]);
  unsigned int width  = *(unsigned int*)&(header[12]);
  unsigned int linear_size = *(unsigned int*)&(header[16]);
  unsigned int mipmap_count = *(unsigned int*)&(header[24]);
  unsigned int four_cc = *(unsigned int*)&(header[80]);

  unsigned char* buffer;
  unsigned int buf_size;
  buf_size = mipmap_count > 1 ? linear_size * 2 : linear_size;
  buffer = (unsigned char*)malloc(buf_size * sizeof(unsigned char));
  fread(buffer, 1, buf_size, fp);
  fclose(fp);

//  unsigned int components = (four_cc == FOURCC_DXT1) ? 3 : 4;
  unsigned int format;
  switch(four_cc)
  {
  case FOURCC_DXT1:
    format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    break;
  case FOURCC_DXT3:
    format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    break;
  case FOURCC_DXT5:
    format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    break;
  default:
    free(buffer);
    return 0;
  }

  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  unsigned int block_size = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 :
                                                                           16;
  unsigned int offset = 0;

  for (unsigned int level = 0; level < mipmap_count && (width || height);
       ++level) {
    unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * block_size;
    glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0,
                           size, buffer + offset);
    offset += size;
    width /= 2;
    height /= 2;

    if (width < 1) width = 1;
    if (height < 1) height = 1;
  }

  free(buffer);

  return texture_id;
}
