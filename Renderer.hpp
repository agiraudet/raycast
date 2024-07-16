#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "RaylibWrapper.hpp"
#include <cstdint>
#include <raylib.h>

class Rend {
public:
  Rend();
  Rend(int width, int height);
  ~Rend();

  void putPixel(int x, int y, uint32_t color);
  void putPixel(int x, int y, rl::Color color);
  void render(void);

private:
  int _width;
  int _height;
  rl::Color *_buf;
  rl::Texture2D _img;
};

#endif
