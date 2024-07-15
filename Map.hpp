#ifndef MAP_HPP
#define MAP_HPP

#include <cstddef>
#include <string>

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

private:
  size_t _width;
  size_t _height;
  char *_tiles;
};

#endif
