#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Texture.hpp"
#include "Window.hpp"

class Map;
class Sprite;

class Player {
public:
  Player();
  Player(double x, double y);
  Player(const Player &other);
  ~Player();
  Player &operator=(const Player &other);

  inline double getPosX(void) { return _posX; };
  inline double getPosY(void) { return _posY; };
  inline double getDirX(void) { return _dirX; };
  inline double getDirY(void) { return _dirY; };
  inline double getPlanX(void) { return _planX; };
  inline double getPlanY(void) { return _planY; };
  inline double getZVal(int i) { return _Zbuffer[i]; }
  void setX(double x);
  void setY(double y);
  void setPos(double x, double y);
  void move(double deltaX, double deltaY);
  void render();
  void raycast(Rend &rend, Map &map);
  void rotate(int dir);
  void move(Map &map);

private:
  double _emitRay(Rend &rend, Map &map, int x);
  void _drawStrip(Rend &rend, double perpWallDist, int texX, Texture &tex,
                  int side, int x);

private:
  double _posX;
  double _posY;
  double _Zbuffer[SCREEN_WIDTH];
  double _moveSpeed;
  double _rotSpeed;
  double _dirX;
  double _dirY;
  double _planX;
  double _planY;
};

#endif // ENTITY_HPP
