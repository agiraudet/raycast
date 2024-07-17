#ifndef MAP_HPP
#define MAP_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "Player.hpp"
#include "Sprite.hpp"
#include "Texture.hpp"

#define CELL_SIZE 40

class Map {
public:
  Map(void);
  Map(Map const &src);
  Map(size_t width, size_t height);
  Map(const std::string &filePath);
  Map &operator=(const Map &other);
  ~Map(void);

  friend std::ostream &operator<<(std::ostream &os, const Map &map);

  char at(size_t x, size_t y);
  void render(size_t cellSize, size_t posX, size_t posY);
  Texture &getTex(char c);
  void setTex(char c, Texture tex);
  Player &getPlayer(void);
  void drawSprites(Rend &rend);
  void drawFloor(Rend &rend);

private:
  void _loadFromFile(const std::string &filePath);
  void _loadTexFromLines(const std::vector<std::string> &lines);
  void _loadMapFromLines(const std::vector<std::string> &lines);
  void _initMapContent(void);

private:
  size_t _width;
  size_t _height;
  char *_tiles;
  std::unordered_map<char, Texture> _texMap;
  std::vector<Sprite> _sprtVec;
  Player _player;
};

#endif
