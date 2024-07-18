#include <cstdlib>
#include <iostream>
#include <ostream>

#include "Anim.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "Window.hpp"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Need a lvl file" << std::endl;
    exit(EXIT_FAILURE);
  }

  Map map(argv[1]);
  rl::InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ray");
  rl::SetTargetFPS(30);
  Rend rend;
  Player &plr = map.getPlayer();
  Anim gun("tex/shoot2.xpm", 5);
  gun.setFps(30);

  while (!rl::WindowShouldClose()) {
    if (IsKeyDown(rl::KEY_SPACE))
      gun.playOnce();
    plr.move(map);
    map.drawFloor(rend);
    plr.raycast(rend, map);
    map.drawSprites(rend);
    gun.update(rend, SCREEN_WIDTH / 2 - gun.getFrameWidth() / 2,
               SCREEN_HEIGHT - gun.getFrameHeight() - 1);
    rend.render();
  }
  return 0;
}

/*int main2(int argc, char **argv) {*/
/*  if (argc < 6) {*/
/*    std::cerr << "Need an vl file and a few tex file :" << std::endl*/
/*              << argv[0]*/
/*              << " [lvl] [wall.xpm] [floor.xmp] [ceiling.xpm] [sprite.xmp]"*/
/*              << std::endl;*/
/*    exit(EXIT_FAILURE);*/
/*  }*/
/**/
/*  Map map;*/
/*  try {*/
/*    map = Map(argv[1]);*/
/*  } catch (const std::exception &e) {*/
/*    std::cerr << "Error: " << e.what() << std::endl;*/
/*    exit(EXIT_FAILURE);*/
/*  }*/
/*  map.setTex('w', Texture(argv[2]));*/
/*  map.setTex('f', Texture(argv[3]));*/
/*  map.setTex('c', Texture(argv[4]));*/
/*  std::vector<Sprite> sprtVec;*/
/**/
/*  sprtVec.push_back(Sprite(Texture(argv[5]), 10, 16));*/
/*  sprtVec.push_back(Sprite(Texture(argv[5]), 12, 18));*/
/*  Player plr(10, 15);*/
/**/
/*  std::cout << map << std::endl;*/
/**/
/*  rl::InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ray");*/
/*  rl::SetTargetFPS(30);*/
/*  Rend rend;*/
/*  Anim gun("tex/slice80.xpm", 5);*/
/**/
/*  while (!rl::WindowShouldClose()) {*/
/*    if (IsKeyDown(rl::KEY_SPACE))*/
/*      gun.playOnce();*/
/*    plr.move(map);*/
/*    plr.raycast(rend, map);*/
/*    plr.drawSprite(rend, sprtVec);*/
/*    gun.update(rend, SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT - 81);*/
/*    rend.render();*/
/*  }*/
/*}*/
