#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Player.hpp"
#include "Texture.hpp"
#include <vector>

class Sprite {
public:
  static void sortVec(std::vector<Sprite> &spriteVec, Player &plr);

public:
  Sprite(Texture tex, double x, double y);
  ~Sprite();

  inline double getX(void) { return _x; }
  inline double getY(void) { return _y; }
  inline Texture &getTex(void) { return _tex; }
  inline double getDist(void) { return _dist; }
  inline void setDist(double dist) { _dist = dist; }

  void draw(Rend &rend, Player &plr);

private:
  Texture _tex;
  double _x;
  double _y;
  double _dist;
};

#endif
