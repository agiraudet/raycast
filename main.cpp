#include "Entity.hpp"
#include "Map.hpp"
#include "RaylibWrapper.hpp"
#include "Texture.hpp"
#include "Window.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
int main(int argc, char **argv) {
  if (argc < 3) {
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
  Entity plr(10, 15);
  Texture tex(argv[2]);

  std::cout << map << std::endl;

  rl::InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ray");
  rl::SetTargetFPS(30);
  while (!rl::WindowShouldClose()) {
    plr.move(map);
    rl::BeginDrawing();
    ClearBackground(rl::BLACK);
    plr.raycast(map);
    map.render(5, 10, 10);
    tex.draw(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    rl::EndDrawing();
  }
}
