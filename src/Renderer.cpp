#include <cstdint>
#include <cstring>

#include "Renderer.hpp"
#include "Window.hpp"

Rend::Rend() : _width(SCREEN_WIDTH), _height(SCREEN_HEIGHT), _buf(nullptr) {
  _buf = new rl::Color[_width * _height];
  std::memset(_buf, 0, _width * _height * sizeof(rl::Color));
  rl::Image tmp = rl::GenImageColor(_width, _height, rl::BLACK);
  _img = rl::LoadTextureFromImage(tmp);
}

Rend::Rend(int width, int height)
    : _width(width), _height(height), _buf(nullptr) {
  _buf = new rl::Color[_width * _height];
  std::memset(_buf, 0, _width * _height * sizeof(rl::Color));
  rl::Image tmp = rl::GenImageColor(_width, _height, rl::BLACK);
  _img = rl::LoadTextureFromImage(tmp);
}

Rend::~Rend() {
  if (_buf)
    delete[] _buf;
  rl::UnloadTexture(_img);
}

rl::Color Rend::uint32ToRGBA(uint32_t color) {
  rl::Color rlColor;
  rlColor.r = (color >> 16) & 0xFF;
  rlColor.g = (color >> 8) & 0xFF;
  rlColor.b = color & 0xFF;
  rlColor.a = color ? 255 : 0;
  return rlColor;
}

rl::Color Rend::reduceLum(rl::Color color, int lumMod) {
  color.r /= lumMod;
  color.g /= lumMod;
  color.b /= lumMod;
  return color;
}

rl::Color Rend::reduceLum(uint32_t color, int lumMod) {
  rl::Color clr = uint32ToRGBA(color);
  return reduceLum(clr, lumMod);
}

void Rend::putPixel(int x, int y, uint32_t color) {
  if (x >= _width || x < 0 || y >= _height || y < 0)
    return;
  rl::Color &col = _buf[y * _width + x];
  col.r = (color >> 16) & 0xFF;
  col.g = (color >> 8) & 0xFF;
  col.b = color & 0xFF;
  col.a = color ? 255 : 0;
}

void Rend::putPixel(int x, int y, rl::Color color) {
  if (x >= _width || x < 0 || y >= _height || y < 0)
    return;
  _buf[y * _width + x] = color;
}

void Rend::render(void) {
  rl::UpdateTexture(_img, _buf);
  rl::BeginDrawing();
  rl::DrawTexture(_img, 0, 0, rl::WHITE);
  rl::EndDrawing();
}
