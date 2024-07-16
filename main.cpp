#include "Entity.hpp"
#include "Map.hpp"
#include "RaylibWrapper.hpp"
#include "Renderer.hpp"
#include "Sprite.hpp"
#include "Texture.hpp"
#include "Window.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 5) {
    std::cerr << "Need an lvl file and a tex file" << std::endl;
    exit(EXIT_FAILURE);
  }

  Map map;
  try {
    map = Map(argv[1]);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  map.setTex('w', Texture(argv[2]));
  map.setTex('f', Texture(argv[3]));
  map.setTex('c', Texture(argv[4]));
  std::vector<Sprite> sprtVec;

  sprtVec.push_back(Sprite(Texture(argv[5]), 10, 12));
  sprtVec.push_back(Sprite(Texture(argv[5]), 12, 12));
  Entity plr(10, 15);

  std::cout << map << std::endl;

  rl::InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ray");
  rl::SetTargetFPS(30);
  Rend rend;
  while (!rl::WindowShouldClose()) {
    plr.move(map);
    plr.raycast(rend, map);
    plr.drawSprite(rend, sprtVec);
    rend.render();
  }
}
