#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>

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

  double getX() const;
  double getY() const;
  void setX(double x);
  void setY(double y);
  void setPos(double x, double y);
  void move(double deltaX, double deltaY);
  void render();
  void raycast(Rend &rend, Map &map);
  void rotate(int dir);
  void move(Map &map);
  void drawSprite(Rend &rend, std::vector<Sprite> &spriteVec);

private:
  double _emitRay(Rend &rend, Map &map, int x);
  /*void _drawStrip(double perpWallDist, int side, int x);*/
  void _drawStrip(Rend &rend, double perpWallDist, int texX, Texture &tex,
                  int side, int x);
  void castFloor(Rend &rend, Texture &texFloor, Texture &texCeil);
  void sortSprites(std::vector<Sprite> &spriteVec);

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
