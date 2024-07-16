#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Map.hpp"
#include "Renderer.hpp"
#include "Sprite.hpp"
#include "Window.hpp"
#include <vector>

class Entity {
public:
  Entity();
  Entity(double x, double y);
  Entity(const Entity &other);
  ~Entity();
  Entity &operator=(const Entity &other);

  double getX() const;
  double getY() const;
  void setX(double x);
  void setY(double y);
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
  double _Zbuffer[SCREEN_WIDTH];
  double _moveSpeed;
  double _rotSpeed;
  double _posX;
  double _posY;
  double _dirX;
  double _dirY;
  double _planX;
  double _planY;
};

#endif // ENTITY_HPP
