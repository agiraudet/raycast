#include "Anim.hpp"

Anim::Anim()
    : Texture(), _frame(0), _nFrames(0), _frameWidth(0), _frameHeight(0),
      _lastFrameTime(0), _fps(10), _cd(0.1), _playing(false), _loop(false) {
  // Default constructor
}

Anim::Anim(size_t frameWidth, size_t frameHeight, size_t nFrames)
    : Texture(frameWidth * nFrames, frameHeight), _frame(0), _nFrames(nFrames),
      _frameWidth(frameWidth), _frameHeight(frameHeight), _lastFrameTime(0),
      _fps(10), _cd(0.1), _playing(false), _loop(false) {
  // Constructor with frame dimensions and number of frames
}

Anim::Anim(const std::string &filePath, size_t frameWidth, size_t frameHeight)
    : Texture(filePath), _frame(0), _frameWidth(frameWidth),
      _frameHeight(frameHeight), _lastFrameTime(0), _fps(10), _cd(0.1),
      _playing(false), _loop(false) {
  if (_frameWidth == 0)
    _frameWidth = _width;
  if (_frameHeight == 0)
    _frameHeight = _height;
  if (_frameWidth != _width)
    _nFrames = _width / _frameWidth;
  else if (_frameHeight != _height)
    _nFrames = _height / _frameHeight;
}

Anim::Anim(const std::string &filePath, size_t nFrames)
    : Texture(filePath), _frame(0), _nFrames(nFrames), _lastFrameTime(0),
      _fps(10), _cd(0.1), _playing(false), _loop(false) {
  if (_width > _height) {
    _frameHeight = _height;
    _frameWidth = _width / _nFrames;
  } else {
    _frameWidth = _width;
    _frameHeight = _height / _nFrames;
  }
}

Anim::Anim(const Anim &src)
    : Texture(src), _frame(src._frame), _nFrames(src._nFrames),
      _frameWidth(src._frameWidth), _frameHeight(src._frameHeight),
      _lastFrameTime(0), _fps(src._fps), _cd(src._cd), _playing(src._playing),
      _loop(src._loop) {}

Anim &Anim::operator=(const Anim &other) {
  if (this != &other) {
    Texture::operator=(other); // Call base class assignment operator
    _frame = other._frame;
    _nFrames = other._nFrames;
    _frameWidth = other._frameWidth;
    _frameHeight = other._frameHeight;
    _lastFrameTime = other._lastFrameTime;
    _fps = other._fps;
    _cd = other._cd;
    _playing = other._playing;
    _loop = other._loop;
  }
  return *this;
}

Anim::~Anim() {
  // Destructor
}

void Anim::nextFrame(void) { _frame = _frame < _nFrames - 1 ? _frame + 1 : 0; }

void Anim::drawFrame(Rend &rend, int posX, int posY) {
  if (_width > _height)
    drawPart(rend, _frameWidth * _frame, 0, _frameWidth, _frameHeight, posX,
             posY);
  else
    drawPart(rend, 0, _frameHeight * _frame, _frameWidth, _frameHeight, posX,
             posY);
}

void Anim::setFps(int fps) {
  _fps = fps;
  _cd = 1.0 / fps;
}

void Anim::playOnce(void) {
  _playing = true;
  _loop = false;
}

void Anim::playLoop(void) {
  _playing = true;
  _loop = true;
}

void Anim::stop(void) {
  _playing = false;
  _loop = false;
}

void Anim::update(Rend &rend, int posX, int posY) {
  drawFrame(rend, posX, posY);
  if (_playing && rl::GetTime() - _lastFrameTime >= _cd) {
    nextFrame();
    if (_frame == 0 && !_loop)
      _playing = false;
    _lastFrameTime = rl::GetTime();
  }
}
