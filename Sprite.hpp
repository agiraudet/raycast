#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Texture.hpp"

class Sprite {
public:
  Sprite(Texture tex, int x, int y);
  ~Sprite();

  inline int getX(void) { return _x; }
  inline int getY(void) { return _y; }
  inline Texture &getTex(void) { return _tex; }
  inline int getTexWidth(void) { return _tex.getWidth(); }
  inline int getTexHeight(void) { return _tex.getHeight(); }
  inline double getDist(void) { return _dist; }
  inline void setDist(double dist) { _dist = dist; }

private:
  Texture _tex;
  int _x;
  int _y;
  double _dist;
};

#endif
