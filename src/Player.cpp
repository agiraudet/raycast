#include "Player.hpp"
#include "Map.hpp"

#include "Window.hpp"
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <vector>

Player::Player()
    : _posX(0.0), _posY(0.0), _moveSpeed(0.2), _rotSpeed(0.1), _dirX(-1),
      _dirY(0), _planX(0), _planY(0.66) {}

Player::Player(double x, double y)
    : _posX(x), _posY(y), _moveSpeed(0.2), _rotSpeed(0.1), _dirX(-1), _dirY(0),
      _planX(0), _planY(0.66) {}

Player::Player(const Player &other)
    : _posX(other._posX), _posY(other._posY), _moveSpeed(other._moveSpeed),
      _rotSpeed(other._rotSpeed), _dirX(other._dirX), _dirY(other._dirY),
      _planX(other._planX), _planY(other._planY) {}

Player::~Player() {}

Player &Player::operator=(const Player &other) {
  if (this == &other) {
    return *this;
  }
  _posX = other._posX;
  _posY = other._posY;
  _moveSpeed = other._moveSpeed;
  _rotSpeed = other._rotSpeed;
  _dirX = other._dirX;
  _dirY = other._dirY;
  _planX = other._planX;
  _planY = other._planY;
  return *this;
}

double Player::getX() const { return _posX; }

double Player::getY() const { return _posY; }

void Player::setX(double x) { _posX = x; }

void Player::setY(double y) { _posY = y; }

void Player::setPos(double x, double y) {
  _posX = x;
  _posY = y;
}

void Player::move(double deltaX, double deltaY) {
  _posX += deltaX;
  _posY += deltaY;
}

void Player::render(void) { DrawCircle(_posX, _posY, 20., rl::RED); }

void Player::raycast(Rend &rend, Map &map) {
  castFloor(rend, map.getTex('.'), map.getTex(','));
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    _emitRay(rend, map, x);
  }
}

void Player::rotate(int dir) {
  /*dir = dir > 0 ? 1 : -1;*/
  double odlDirX = _dirX;
  _dirX = _dirX * cos(_rotSpeed * dir) - _dirY * sin(_rotSpeed * dir);
  _dirY = odlDirX * sin(_rotSpeed * dir) + _dirY * cos(_rotSpeed * dir);
  double oldPlanX = _planX;
  _planX = _planX * cos(_rotSpeed * dir) - _planY * sin(_rotSpeed * dir);
  _planY = oldPlanX * sin(_rotSpeed * dir) + _planY * cos(_rotSpeed * dir);
}

void Player::move(Map &map) {
  if (IsKeyDown(rl::KEY_W)) {
    if (map.at(_posX + _dirX * _moveSpeed, _posY) == '.')
      _posX += _dirX * _moveSpeed;
    if (map.at(_posX, _posY + _dirY * _moveSpeed) == '.')
      _posY += _dirY * _moveSpeed;
  }
  if (IsKeyDown(rl::KEY_S)) {
    if (map.at(_posX - _dirX * _moveSpeed, _posY) == '.')
      _posX -= _dirX * _moveSpeed;
    if (map.at(_posX, _posY - _dirY * _moveSpeed) == '.')
      _posY -= _dirY * _moveSpeed;
  }
  /*if (IsKeyDown(rl::KEY_Q)) {*/
  /*  float strafeX = -_dirY * _moveSpeed;*/
  /*  float strafeY = _dirX * _moveSpeed;*/
  /*  if (map.at(_posX + strafeX, _posY) == '.')*/
  /*    _posX += strafeX;*/
  /*  if (map.at(_posX, _posY + strafeY) == '.')*/
  /*    _posY += strafeY;*/
  /*}*/
  /*if (IsKeyDown(rl::KEY_E)) {*/
  /*  float strafeX = _dirY * _moveSpeed;*/
  /*  float strafeY = -_dirX * _moveSpeed;*/
  /*  if (map.at(_posX + strafeX, _posY) == '.')*/
  /*    _posX += strafeX;*/
  /*  if (map.at(_posX, _posY + strafeY) == '.')*/
  /*    _posY += strafeY;*/
  /*}*/
  if (IsKeyDown(rl::KEY_D)) {
    rotate(-1);
  }
  if (IsKeyDown(rl::KEY_A)) {
    rotate(1);
  }
}

void Player::castFloor(Rend &rend, Texture &texFloor, Texture &texCeil) {
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
      rend.putPixel(x, y, rend.reduceLum(texFloor.getPix(tx, ty), 2));
      rend.putPixel(x, SCREEN_HEIGHT - y - 1,
                    rend.reduceLum(texCeil.getPix(tx, ty), 2));
    }
  }
}

double Player::_emitRay(Rend &rend, Map &map, int x) {
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
  _Zbuffer[x] = perpWallDist;
  return 0;
}

void Player::_drawStrip(Rend &rend, double perpWallDist, int texX, Texture &tex,
                        int side, int x) {
  int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
  int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
  int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;

  if (side == 0)
    tex.drawStrip(rend, drawStart, drawEnd, texX, x, 2);
  else
    tex.drawStrip(rend, drawStart, drawEnd, texX, x);
}

void Player::sortSprites(std::vector<Sprite> &spriteVec) {
  for (auto &sprite : spriteVec) {
    double dist = ((_posX - sprite.getX()) * (_posX - sprite.getX()) +
                   (_posY - sprite.getY()) * (_posY * sprite.getY()));
    sprite.setDist(dist);
  }
  std::sort(spriteVec.begin(), spriteVec.end(),
            [](Sprite &a, Sprite &b) { return a.getDist() > b.getDist(); });
}

void Player::drawSprite(Rend &rend, std::vector<Sprite> &spriteVec) {
  sortSprites(spriteVec);

  for (auto &sprite : spriteVec) {

    double spriteX = sprite.getX() - _posX;
    double spriteY = sprite.getY() - _posY;
    double invDet = 1.0 / (_planX * _dirY - _dirX * _planY);
    double transformX = invDet * (_dirY * spriteX - _dirX * spriteY);
    double transformY = invDet * (-_planY * spriteX + _planX * spriteY);

    int spriteScreenX = int((SCREEN_WIDTH / 2) * (1 + transformX / transformY));
    int spriteHeight = abs(int(SCREEN_HEIGHT / (transformY)));
    int drawStartY = -spriteHeight / 2 + SCREEN_HEIGHT / 2;
    /*drawStartY = drawStartY < 0 ? 0 : drawStartY;*/
    int drawEndY = spriteHeight / 2 + SCREEN_HEIGHT / 2;
    /*drawEndY = drawEndY >= SCREEN_HEIGHT ? SCREEN_HEIGHT - 1 : drawEndY;*/

    int spriteWidth = abs(int(SCREEN_HEIGHT / (transformY)));
    int drawStartX = -spriteWidth / 2 + spriteScreenX;
    drawStartX = drawStartX < 0 ? 0 : drawStartX;
    int drawEndX = spriteWidth / 2 + spriteScreenX;
    drawEndX = drawEndX >= SCREEN_WIDTH ? SCREEN_WIDTH - 1 : drawEndX;

    for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
      int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) *
                     sprite.getTexWidth() / spriteWidth) /
                 256;
      if (transformY > 0 && stripe > 0 && stripe < SCREEN_WIDTH &&
          transformY < _Zbuffer[stripe]) {
        sprite.getTex().drawStrip(rend, drawStartY, drawEndY, texX, stripe);
      }
    }
  }
}
