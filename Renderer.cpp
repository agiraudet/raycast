#include "Renderer.hpp"
#include "Window.hpp"
#include <cstring>
#include <raylib.h>

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

void Rend::putPixel(int x, int y, uint32_t color) {
  rl::Color &col = _buf[y * _width + x];
  col.r = (color >> 16) & 0xFF;
  col.g = (color >> 8) & 0xFF;
  col.b = color & 0xFF;
  col.a = color ? 255 : 0;
}

void Rend::putPixel(int x, int y, rl::Color color) {
  _buf[y * _width + x] = color;
}

void Rend::render(void) {
  rl::UpdateTexture(_img, _buf);
  rl::BeginDrawing();
  rl::DrawTexture(_img, 0, 0, rl::WHITE);
  rl::EndDrawing();
}
