#include <algorithm>

#include "Sprite.hpp"

Sprite::Sprite(Texture tex, double x, double y) : _tex(tex), _x(x), _y(y) {}

Sprite::~Sprite() {}

void Sprite::draw(Rend &rend, Player &plr) {

  double spriteX = _x - plr.getPosX();
  double spriteY = _y - plr.getPosY();
  double invDet =
      1.0 / (plr.getPlanX() * plr.getDirY() - plr.getDirX() * plr.getPlanY());
  double transformX =
      invDet * (plr.getDirY() * spriteX - plr.getDirX() * spriteY);
  double transformY =
      invDet * (-plr.getPlanY() * spriteX + plr.getPlanX() * spriteY);

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
                   _tex.getWidth() / spriteWidth) /
               256;
    if (transformY > 0 && stripe > 0 && stripe < SCREEN_WIDTH &&
        transformY < plr.getZVal(stripe)) {
      _tex.drawStrip(rend, drawStartY, drawEndY, texX, stripe);
    }
  }
}

void Sprite::sortVec(std::vector<Sprite> &spriteVec, Player &plr) {
  for (auto &sprite : spriteVec) {
    double dist =
        ((plr.getPosX() - sprite.getX()) * (plr.getPosX() - sprite.getX()) +
         (plr.getPosY() - sprite.getY()) * (plr.getPosY() * sprite.getY()));
    sprite.setDist(dist);
  }
  std::sort(spriteVec.begin(), spriteVec.end(),
            [](Sprite &a, Sprite &b) { return a.getDist() > b.getDist(); });
}
