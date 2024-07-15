#include "Texture.hpp"
#include "XpmData.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

Texture::Texture(void) : _width(0), _height(0), _tex(nullptr) {}

Texture::Texture(size_t width, size_t height) : _width(width), _height(height) {
  _tex = new uint32_t[width * height];
}

Texture::Texture(Texture const &src)
    : _width(src._width), _height(src._height), _tex(nullptr) {
  if (src._tex) {
    _tex = new uint32_t[_width * _height];
    std::memcpy(_tex, src._tex, _width * _height * sizeof(uint32_t));
  }
}

Texture::Texture(const std::string &filePath) {
  XpmData xpm(filePath);
  _width = xpm.getWidth();
  _height = xpm.getHeight();
  _tex = new uint32_t[_width * _height];
  std::memcpy(_tex, xpm.getData(), _width * _height * sizeof(uint32_t));
}

Texture &Texture::operator=(const Texture &other) {
  if (this == &other)
    return *this;
  delete[] _tex;
  _tex = nullptr;
  _width = other._width;
  _height = other._height;
  if (other._tex) {
    _tex = new uint32_t[_width * _height];
    std::memcpy(_tex, other._tex, _width * _height * sizeof(uint32_t));
  }
  return *this;
}

Texture::~Texture(void) {
  if (_tex)
    delete[] _tex;
}

rl::Color Texture::uint32ToRGBA(uint32_t color) {
  rl::Color rlColor;
  rlColor.r = (color >> 16) & 0xFF;
  rlColor.g = (color >> 8) & 0xFF;
  rlColor.b = color & 0xFF;
  rlColor.a = color ? 255 : 0;
  return rlColor;
}

void Texture::draw(int posX, int posY) {
  for (size_t y = 0; y < _height; y++) {
    for (size_t x = 0; x < _width; x++) {
      rl::DrawPixel(posX + x, posY + y, uint32ToRGBA(_tex[y * _width + x]));
    }
  }
}
