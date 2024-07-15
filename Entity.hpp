#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Map.hpp"

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
  void render(void);
  void raycast(Map &map);
  void rotate(int dir);
  void move(Map &map);

private:
  double _emitRay(Map &map, int x, int &side);
  void _drawStrip(double perpWallDist, int side, int x);

private:
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
