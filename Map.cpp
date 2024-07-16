#include "Map.hpp"
#include "RaylibWrapper.hpp"
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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

Map::Map(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file)
    throw std::runtime_error("Unable to open file");

  std::string line;
  std::vector<std::string> lines;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }

  if (lines.empty())
    throw std::runtime_error("File is empty");

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
