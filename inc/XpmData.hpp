#ifndef XPMDATA_HPP
#define XPMDATA_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class XpmData {

public:
  XpmData();
  XpmData(XpmData const &src);
  XpmData(const std::string &filePath);
  XpmData &operator=(const XpmData &other);
  ~XpmData();

  bool loadFromFile(const std::string &filePath);
  size_t getWidth(void);
  size_t getHeight(void);
  uint32_t *getData(void);

private:
  bool _readHeader(void);
  bool _readColorMap(void);
  bool _readData(void);

private:
  size_t _nColors;
  size_t _charPerPixel;
  size_t _width;
  size_t _height;
  size_t _headerPos;
  size_t _colorMapPos;
  size_t _dataPos;
  std::unordered_map<std::string, uint32_t> _colorMap;
  std::vector<std::string> _rawData;
  uint32_t *_data;
};

#endif // !XPMDATA_HPP
