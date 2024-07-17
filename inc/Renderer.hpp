#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstdint>

#include "RaylibWrapper.hpp"

class Rend {
public:
  Rend();
  Rend(int width, int height);
  ~Rend();

  void putPixel(int x, int y, uint32_t color);
  void putPixel(int x, int y, rl::Color color);
  void render(void);
  rl::Color uint32ToRGBA(uint32_t color);
  rl::Color reduceLum(rl::Color color, int lumMod);
  rl::Color reduceLum(uint32_t color, int lumMod);

private:
  int _width;
  int _height;
  rl::Color *_buf;
  rl::Texture2D _img;
};

#endif
