#include "XpmData.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

XpmData::XpmData()
    : _nColors(0), _charPerPixel(0), _width(0), _height(0), _headerPos(0),
      _colorMapPos(0), _dataPos(0), _data(nullptr) {}

XpmData::XpmData(const std::string &filePath)
    : _nColors(0), _charPerPixel(0), _width(0), _height(0), _headerPos(0),
      _colorMapPos(0), _dataPos(0), _data(nullptr) {
  if (!loadFromFile(filePath))
    throw std::runtime_error("Unable to load .xpm file");
}

XpmData::XpmData(XpmData const &src)
    : _nColors(src._nColors), _charPerPixel(src._charPerPixel),
      _width(src._width), _height(src._height), _headerPos(src._headerPos),
      _colorMapPos(src._colorMapPos), _dataPos(src._dataPos),
      _colorMap(src._colorMap), _rawData(src._rawData), _data(nullptr) {
  if (src._data) {
    _data = new uint32_t[_width * _height];
    std::memcpy(_data, src._data, _width * _height * sizeof(uint32_t));
  }
}

XpmData &XpmData::operator=(const XpmData &other) {
  if (this != &other) {
    _nColors = other._nColors;
    _charPerPixel = other._charPerPixel;
    _width = other._width;
    _height = other._height;
    _headerPos = other._headerPos;
    _colorMapPos = other._colorMapPos;
    _dataPos = other._dataPos;
    _colorMap = other._colorMap;
    _rawData = other._rawData;
    delete[] _data;
    _data = nullptr;
    if (other._data) {
      _data = new uint32_t[_width * _height];
      std::memcpy(_data, other._data, _width * _height * sizeof(uint32_t));
    }
  }
  return *this;
}

XpmData::~XpmData() {
  if (_data)
    delete[] _data;
}

size_t XpmData::getWidth(void) { return _width; }

size_t XpmData::getHeight(void) { return _height; }

uint32_t *XpmData::getData(void) { return _data; }

bool XpmData::loadFromFile(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    std::cerr << "Texture loading failed:" << filePath << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(file, line))
    _rawData.push_back(line);
  file.close();

  if (_rawData.size() < 2) {
    std::cerr << "Invalid XPM file:" << filePath << std::endl;
    return false;
  }
  return _readHeader() && _readColorMap() && _readData();
}

bool XpmData::_readHeader(void) {
  while (_headerPos < _rawData.size() && _rawData[_headerPos][0] != '"')
    _headerPos++;
  if (_headerPos >= _rawData.size()) {
    std::cerr << "XpmData: Could not find header." << std::endl;
    return false;
  }
  std::istringstream headerStream(_rawData[_headerPos]);
  headerStream.ignore(1);
  headerStream >> _width >> _height >> _nColors >> _charPerPixel;
  std::cout << "header: " << _headerPos << _width << _height << _nColors
            << _charPerPixel << std::endl;
  if (_data)
    delete[] _data;
  _data = new uint32_t[_width * _height];
  return true;
}

bool XpmData::_readColorMap(void) {
  _colorMapPos = _headerPos + 1;
  while (_colorMapPos < _rawData.size() && _rawData[_colorMapPos][0] != '"')
    _colorMapPos++;
  if (_colorMapPos >= _rawData.size()) {
    std::cerr << "XpmData: Could not find color map." << std::endl;
    return false;
  }
  for (size_t i = 0; i < _nColors; i++) {
    std::string colorEntry = _rawData[i + _colorMapPos];
    std::string colorSymbol = colorEntry.substr(1, _charPerPixel);
    if (colorEntry[_charPerPixel + 4] != '#')
      _colorMap[colorSymbol] = 0;
    else {
      std::string colorValue = colorEntry.substr(_charPerPixel + 5);
      _colorMap[colorSymbol] = std::stoul(colorValue, nullptr, 16);
    }
    std::cout << "color: |" << colorSymbol << "| " << _colorMap[colorSymbol]
              << std::endl;
  }
  return true;
}

bool XpmData::_readData(void) {
  _dataPos = _colorMapPos + _colorMap.size() + 1;
  while (_dataPos < _rawData.size() && _rawData[_dataPos][0] != '"')
    _dataPos++;
  if (_dataPos >= _rawData.size()) {
    std::cerr << "XpmData: Could not find data." << std::endl;
    return false;
  }
  std::cout << "Data: " << _width << " " << _height << " " << _dataPos
            << std::endl;
  for (int y = 0; y < _height; y++) {
    std::string row = _rawData[_dataPos + y];
    for (int x = 0; x < _width; x++) {
      if (row[0] == '"') {
        std::string pixelSymbol =
            row.substr(x * _charPerPixel + 1, _charPerPixel);
        _data[y * _width + x] = _colorMap[pixelSymbol];
      } else {
        _data[y * _width + x] = 0;
      }
    }
  }
  return true;
}
