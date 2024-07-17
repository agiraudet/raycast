#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Map.hpp"
#include "RaylibWrapper.hpp"

Map::Map(void) : _width(0), _height(0), _tiles(nullptr) {}

Map::Map(Map const &src)
    : _width(src._width), _height(src._height), _tiles(nullptr) {
  if (src._tiles) {
    _tiles = new char[_width * _height];
    std::memcpy(_tiles, src._tiles, _width * _height * sizeof(char));
  }
}

Map::Map(size_t width, size_t _height) : _width(width), _height(_height) {
  _tiles = new char[_width * _height];
}

Map::~Map(void) { delete[] _tiles; }

Map::Map(const std::string &filePath) { _loadFromFile(filePath); }

Map &Map::operator=(const Map &other) {
  if (this == &other) {
    return *this;
  }
  delete[] _tiles;
  _width = other._width;
  _height = other._height;
  if (other._tiles) {
    _tiles = new char[_width * _height];
    std::memcpy(_tiles, other._tiles, _width * _height * sizeof(char));
  } else {
    _tiles = nullptr;
  }
  return *this;
}

Player &Map::getPlayer(void) { return _player; }

void Map::drawSprites(Rend &rend) {
  Sprite::sortVec(_sprtVec, _player);

  for (auto &sprite : _sprtVec) {
    sprite.draw(rend, _player);
  }
}

void Map::drawFloor(Rend &rend) {
  Texture &texFloor = _texMap['.'];
  Texture &texCeil = _texMap[','];
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    float rayDirX0 = _player.getDirX() - _player.getPlanX();
    float rayDirY0 = _player.getDirY() - _player.getPlanY();
    float rayDirX1 = _player.getDirX() + _player.getPlanX();
    float rayDirY1 = _player.getDirY() + _player.getPlanY();
    int p = y - SCREEN_HEIGHT / 2;
    float posZ = 0.5 * SCREEN_HEIGHT;
    float rowDist = posZ / p;
    float floorStepX = rowDist * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
    float floorStepY = rowDist * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;
    float floorX = _player.getPosX() + rowDist * rayDirX0;
    float floorY = _player.getPosY() + rowDist * rayDirY0;
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

void Map::_loadFromFile(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file)
    throw std::runtime_error("Unable to open file");

  std::string line;
  std::vector<std::string> mapLines;
  std::vector<std::string> texLines;
  std::vector<std::string> *lines = &texLines;
  while (std::getline(file, line)) {
    if (line.size() <= 1 || line[0] == '#')
      continue;
    else if (line[0] == '!') {
      if (line.compare(1, 3, "Map") == 0)
        lines = &mapLines;
      else if (line.compare(1, 3, "Tex") == 0)
        lines = &texLines;
    } else {
      std::cout << line.size() << "|" << line << std::endl;
      lines->push_back(line);
    }
  }
  if (mapLines.empty())
    throw std::runtime_error("File map section is empty");
  if (texLines.empty())
    throw std::runtime_error("File tex section is empty");
  _loadTexFromLines(texLines);
  _loadMapFromLines(mapLines);
  _initMapContent();
}

void Map::_loadTexFromLines(const std::vector<std::string> &lines) {
  for (const auto &l : lines) {
    std::cout << "Tex|" << l.substr(2) << std::endl;
    setTex(l[0], l.substr(2));
  }
}

void Map::_loadMapFromLines(const std::vector<std::string> &lines) {
  _height = lines.size();
  _width = lines[0].size();

  for (const auto &l : lines) {
    if (l.size() != _width)
      throw std::runtime_error("Inconsistent line length in file");
  }

  _tiles = new char[_width * _height];

  for (size_t i = 0; i < _height; i++) {
    std::memcpy(_tiles + i * _width, lines[i].c_str(), _width * sizeof(char));
  }
}

void Map::_initMapContent(void) {
  for (size_t y = 0; y < _height; y++) {
    for (size_t x = 0; x < _width; x++) {
      switch (_tiles[y * _width + x]) {
      case 'S':
        _sprtVec.push_back(Sprite(getTex('S'), x + 0.5, y + 0.5));
        _tiles[y * _width + x] = '.';
        break;
      case 'X':
        _player.setPos(x + 0.5, y + 0.5);
        _tiles[y * _width + x] = '.';
        break;
      }
    }
  }
}

char Map::at(size_t x, size_t y) {
  if (!_tiles || x >= _width || y >= _height)
    return 0;
  return _tiles[y * _width + x];
}

void Map::render(size_t cellSize, size_t posX, size_t posY) {
  for (size_t y = 0; y < _height; y++) {
    for (size_t x = 0; x < _width; x++) {
      rl::Color color = rl::PINK;
      switch (at(x, y)) {
      case 'w':
        color = rl::WHITE;
        break;
      case '.':
        color = rl::BLACK;
        break;
      }
      rl::DrawRectangle((x * cellSize) + posX, (y * cellSize) + posY, cellSize,
                        cellSize, color);
    }
  }
}

Texture &Map::getTex(char c) { return _texMap[c]; }

void Map::setTex(char c, Texture tex) { _texMap[c] = tex; }

std::ostream &operator<<(std::ostream &os, const Map &map) {
  for (size_t i = 0; i < map._height; ++i) {
    for (size_t j = 0; j < map._width; ++j) {
      os << map._tiles[i * map._width + j];
    }
    os << std::endl;
  }
  return os;
}
