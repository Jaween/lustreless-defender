#ifndef IMAGE_H_
#define IMAGE_H_

#include "GL/glew.h"
#include "SDL_gpu.h"
#include <string>
#include <vector>

#include "camera.h"
#include "texture_shader.h"

/**
 * Represents a single static image.
 */
class Image
{
 public:
  Image(uint32_t width, uint32_t height);
  Image(std::string filename);
  ~Image();
  void draw(GPU_Target* gpu_target, float x, float y, float angle=0,
      Shader* shader=NULL, Camera* camera = NULL, bool bind_texture = true);
  void draw(GPU_Target* gpu_target, float x, float y, uint32_t width,
      uint32_t height, float angle=0, Shader* shader=NULL,
      Camera* camera = NULL, bool bind_texture = true);
  GLuint getTexture();
  uint32_t getWidth() const;
  uint32_t getHeight() const;
  void setBlendingEnabled(bool enabled);
  void setBlendMode(GLenum sfactor, GLenum dfactor);

 private:
  uint32_t width;
  uint32_t height;
  GLuint vertex_array;
  GLuint vertex_buffer;
  GLuint index_buffer;
  GLuint uv_buffer;
  GLuint texture;
  GLenum texture_target;
  TextureShader texture_shader;
  Camera* default_camera;
  bool blending_enabled;
  GLenum blend_sfactor;
  GLenum blend_dfactor;

  void init();
  void setTextureData(uint32_t width, uint32_t height, uint8_t* data);
  void drawInternal(GPU_Target* gpu_target, float x, float y, uint32_t width,
      uint32_t height, float angle, Shader* shader=NULL, Camera* camera = NULL,
      bool bind_texture = true);
};

#endif // MESH_H_
