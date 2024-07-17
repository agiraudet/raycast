#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstddef>
#include <cstdint>
#include <string>

#include "Renderer.hpp"

class Texture {
public:
  Texture();
  Texture(size_t width, size_t height);
  Texture(Texture const &src);
  Texture(const std::string &filePath);
  Texture &operator=(const Texture &other);
  ~Texture();

  int getWidth(void);
  int getHeight(void);
  bool loadFromXPM(const std::string &filePath);
  rl::Color uint32ToRGBA(uint32_t color);
  rl::Color uint32ToRGBA(uint32_t color, int lumMod);
  rl::Color getPixColor(int x, int y);
  uint32_t getPix(int x, int y);
  void draw(Rend &rend, int posX, int posY);
  void drawPart(Rend &rend, int texX, int texY, int w, int h, int posX,
                int posY);
  void drawStrip(Rend &rend, int drawStart, int drawEnd, int texX, int destX);
  void drawStrip(Rend &rend, int drawStart, int drawEnd, int texX, int destX,
                 int lumMod);

private:
  void _resize();

protected:
  size_t _width;
  size_t _height;
  uint32_t *_tex;
};

#endif // !TEXTURE_HPP