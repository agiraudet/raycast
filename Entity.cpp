#include "Entity.hpp"
#include "Window.hpp"
#include <math.h>
#include <raylib.h>

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

void Entity::raycast(Rend &rend, Map &map) {
  castFloor(rend, map.getTex('f'), map.getTex('c'));
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    _emitRay(rend, map, x);
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

void Entity::castFloor(Rend &rend, Texture &texFloor, Texture &texCeil) {
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    float rayDirX0 = _dirX - _planX;
    float rayDirY0 = _dirY - _planY;
    float rayDirX1 = _dirX + _planX;
    float rayDirY1 = _dirY + _planY;
    int p = y - SCREEN_HEIGHT / 2;
    float posZ = 0.5 * SCREEN_HEIGHT;
    float rowDist = posZ / p;
    float floorStepX = rowDist * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
    float floorStepY = rowDist * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;
    float floorX = _posX + rowDist * rayDirX0;
    float floorY = _posY + rowDist * rayDirY0;
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      int cellX = (int)(floorX);
      int cellY = (int)(floorY);
      int tx = (int)(texFloor.getWidth() * (floorX - cellX)) &
               (texFloor.getWidth() - 1);
      int ty = (int)(texFloor.getHeight() * (floorY - cellY)) &
               (texFloor.getHeight() - 1);
      floorX += floorStepX;
      floorY += floorStepY;
      /*rl::DrawPixel(x, y, texFloor.getPixColor(tx, ty));*/
      /*rl::DrawPixel(x, SCREEN_HEIGHT - y - 1, texCeil.getPixColor(tx, ty));*/
      rend.putPixel(x, y, texFloor.getPix(tx, ty));
      rend.putPixel(x, SCREEN_HEIGHT - y - 1, texCeil.getPix(tx, ty));
    }
  }
}

double Entity::_emitRay(Rend &rend, Map &map, int x) {
  double cameraX = 2 * x / double(SCREEN_WIDTH) - 1;
  double rayDirX = _dirX + _planX * cameraX;
  double rayDirY = _dirY + _planY * cameraX;
  int mapX = int(_posX);
  int mapY = int(_posY);
  double sideDistX, sideDistY;
  double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
  double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
  int stepX, stepY;
  char hit = 0;
  int side = 0;

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
      hit = map.at(mapX, mapY);
  }
  double perpWallDist =
      side == 0 ? sideDistX - deltaDistX : sideDistY - deltaDistY;
  double wallX = side == 0 ? _posY + perpWallDist * rayDirY
                           : _posX + perpWallDist * rayDirX;
  wallX -= floor(wallX);
  Texture &tex = map.getTex(hit);
  int texWidth = tex.getWidth();
  int texX = int(wallX * double(texWidth));
  if (side == 0 && rayDirX > 0)
    texX = texWidth - texX - 1;
  if (side == 1 && rayDirY < 0)
    texX = texWidth - texX - 1;
  _drawStrip(rend, perpWallDist, texX, tex, side, x);
  return 0;
}

void Entity::_drawStrip(Rend &rend, double perpWallDist, int texX, Texture &tex,
                        int side, int x) {
  int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
  int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
  int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;

  if (side == 0)
    tex.drawStrip(rend, drawStart, drawEnd, texX, x, 2);
  else
    tex.drawStrip(rend, drawStart, drawEnd, texX, x);
}
