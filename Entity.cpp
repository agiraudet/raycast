#include "Entity.hpp"
#include "RaylibWrapper.hpp"
#include "Window.hpp"
#include <cmath>
#include <math.h>

Entity::Entity() : _posX(0.0), _posY(0.0) {}

Entity::Entity(double x, double y)
    : _moveSpeed(0.3), _rotSpeed(0.1), _posX(x), _posY(y), _dirX(-1), _dirY(0),
      _planX(0), _planY(0.66) {}

Entity::Entity(const Entity &other) : _posX(other._posX), _posY(other._posY) {}

Entity::~Entity() {}

Entity &Entity::operator=(const Entity &other) {
  if (this == &other) {
    return *this;
  }
  _posX = other._posX;
  _posY = other._posY;
  return *this;
}

double Entity::getX() const { return _posX; }

double Entity::getY() const { return _posY; }

void Entity::setX(double x) { _posX = x; }

void Entity::setY(double y) { _posY = y; }

void Entity::move(double deltaX, double deltaY) {
  _posX += deltaX;
  _posY += deltaY;
}

void Entity::render(void) { DrawCircle(_posX, _posY, 20., rl::RED); }

void Entity::raycast(Map &map) {
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    int side = 0;
    double perpWallDist = _emitRay(map, x, side);
    _drawStrip(perpWallDist, side, x);
  }
}

void Entity::rotate(int dir) {
  dir = dir > 0 ? 1 : -1;
  double odlDirX = _dirX;
  _dirX = _dirX * cos(_rotSpeed * dir) - _dirY * sin(_rotSpeed * dir);
  _dirY = odlDirX * sin(_rotSpeed * dir) + _dirY * cos(_rotSpeed * dir);
  double oldPlanX = _planX;
  _planX = _planX * cos(_rotSpeed * dir) - _planY * sin(_rotSpeed * dir);
  _planY = oldPlanX * sin(_rotSpeed * dir) + _planY * cos(_rotSpeed * dir);
}

void Entity::move(Map &map) {
  if (IsKeyDown(rl::KEY_UP)) {
    if (map.at(_posX + _dirX * _moveSpeed, _posY) == '.')
      _posX += _dirX * _moveSpeed;
    if (map.at(_posX, _posY + _dirY * _moveSpeed) == '.')
      _posY += _dirY * _moveSpeed;
  }
  if (IsKeyDown(rl::KEY_DOWN)) {
    if (map.at(_posX - _dirX * _moveSpeed, _posY) == '.')
      _posX -= _dirX * _moveSpeed;
    if (map.at(_posX, _posY - _dirY * _moveSpeed) == '.')
      _posY -= _dirY * _moveSpeed;
  }
  if (IsKeyDown(rl::KEY_RIGHT)) {
    rotate(-1);
  }
  if (IsKeyDown(rl::KEY_LEFT)) {
    rotate(1);
  }
}

double Entity::_emitRay(Map &map, int x, int &side) {
  double cameraX = 2 * x / double(SCREEN_WIDTH) - 1;
  double rayDirX = _dirX + _planX * cameraX;
  double rayDirY = _dirY + _planY * cameraX;
  int mapX = int(_posX);
  int mapY = int(_posY);
  double sideDistX, sideDistY;
  double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
  double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
  int stepX, stepY;
  int hit = 0;

  if (rayDirX < 0) {
    stepX = -1;
    sideDistX = (_posX - mapX) * deltaDistX;
  } else {
    stepX = 1;
    sideDistX = (mapX + 1.0 - _posX) * deltaDistX;
  }
  if (rayDirY < 0) {
    stepY = -1;
    sideDistY = (_posY - mapY) * deltaDistY;
  } else {
    stepY = 1;
    sideDistY = (mapY + 1.0 - _posY) * deltaDistY;
  }
  while (hit == 0) {
    if (sideDistX < sideDistY) {
      sideDistX += deltaDistX;
      mapX += stepX;
      side = 0;
    } else {
      sideDistY += deltaDistY;
      mapY += stepY;
      side = 1;
    }
    if (map.at(mapX, mapY) != '.')
      hit = 1;
  }
  return side == 0 ? sideDistX - deltaDistX : sideDistY - deltaDistY;
}

void Entity::_drawStrip(double perpWallDist, int side, int x) {
  int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
  int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
  if (drawStart < 0)
    drawStart = 0;
  int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
  if (drawEnd >= SCREEN_HEIGHT)
    drawEnd = SCREEN_HEIGHT - 1;

  rl::Color color = side == 1 ? rl::PINK : rl::PURPLE;
  rl::DrawLine(x, drawStart, x, drawEnd, color);
}
