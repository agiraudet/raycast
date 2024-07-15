#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstddef>
#include <cstdint>
#include <string>

namespace rl {
#include "raylib.h"
}

class Texture {
public:
  Texture();
  Texture(size_t width, size_t height);
  Texture(Texture const &src);
  Texture(const std::string &filePath);
  Texture &operator=(const Texture &other);
  ~Texture();

  bool loadFromXPM(const std::string &filePath);
  rl::Color uint32ToRGBA(uint32_t color);
  void draw(int posX, int posY);

private:
  void _resize();

private:
  size_t _width;
  size_t _height;
  uint32_t *_tex;
};

#endif // !TEXTURE_HPP
#define TEXTURE_HPP
