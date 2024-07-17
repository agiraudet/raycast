#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>

#include "Texture.hpp"
#include "Window.hpp"
#include "XpmData.hpp"

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

int Texture::getWidth() { return _width; }

int Texture::getHeight() { return _height; }

rl::Color Texture::uint32ToRGBA(uint32_t color) {
  rl::Color rlColor;
  rlColor.r = (color >> 16) & 0xFF;
  rlColor.g = (color >> 8) & 0xFF;
  rlColor.b = color & 0xFF;
  rlColor.a = color ? 255 : 0;
  return rlColor;
}

rl::Color Texture::uint32ToRGBA(uint32_t color, int lumMod) {
  rl::Color rlColor = uint32ToRGBA(color);
  rlColor.r /= lumMod;
  rlColor.g /= lumMod;
  rlColor.b /= lumMod;
  return rlColor;
}

rl::Color Texture::getPixColor(int x, int y) {
  return uint32ToRGBA(_tex[y * _width + x]);
}

uint32_t Texture::getPix(int x, int y) { return _tex[y * _width + x]; }

void Texture::draw(Rend &rend, int posX, int posY) {
  for (int y = 0; y < (int)_height; y++) {
    for (int x = 0; x < (int)_width; x++) {
      rend.putPixel(posX + x, posY + y, _tex[y * _width + x]);
    }
  }
}

void Texture::drawPart(Rend &rend, int texX, int texY, int w, int h, int posX,
                       int posY) {
  for (int y = texY; y < (int)_height && y < texY + h; y++) {
    for (int x = texX; x < (int)_width && x < texX + w; x++) {
      rend.putPixel(posX + x - texX, posY + y - texY, _tex[y * _width + x]);
    }
  }
}

void Texture::drawStrip(Rend &rend, int drawStart, int drawEnd, int texX,
                        int destX) {
  int drawLen = drawEnd - drawStart;
  double step = (double)_height / (double)drawLen;

  double texY = 0;
  for (int y = drawStart; y < drawEnd && y < SCREEN_HEIGHT; y++) {
    if (y < 0) {
      y = 0;
      texY += step * abs(drawStart);
    }
    uint32_t texPix = _tex[(int)texY * _width + texX];
    if (texPix)
      rend.putPixel(destX, y, texPix);
    texY += step;
  }
}

void Texture::drawStrip(Rend &rend, int drawStart, int drawEnd, int texX,
                        int destX, int lumMod) {
  int drawLen = drawEnd - drawStart;
  double step = (double)_height / (double)drawLen;

  double texY = 0;
  for (int y = drawStart; y < drawEnd && y < SCREEN_HEIGHT; y++) {
    if (y < 0) {
      y = 0;
      texY += step * abs(drawStart);
    }
    uint32_t texPix = _tex[(int)texY * _width + texX];
    rend.putPixel(destX, y, uint32ToRGBA(texPix, lumMod));
    texY += step;
  }
}
