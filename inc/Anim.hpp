#ifndef ANIM_HPP
#define ANIM_HPP

#include "Texture.hpp"

class Anim : public Texture {
public:
  Anim();
  Anim(size_t frameWidth, size_t frameHeight, size_t nFrames);
  Anim(const std::string &filePath, size_t frameWidth, size_t frameHeight);
  Anim(const std::string &filePath, size_t nFrames);
  Anim(const Anim &src);
  Anim &operator=(const Anim &other);
  ~Anim();

  void nextFrame(void);
  void drawFrame(Rend &rend, int posX, int posY);
  void setFps(int fps);
  void playOnce(void);
  void playLoop(void);
  void stop(void);
  void update(Rend &rend, int posX, int posY);

  inline size_t getFrameWidth(void) { return _frameWidth; }
  inline size_t getFrameHeight(void) { return _frameHeight; }

private:
  size_t _frame;
  size_t _nFrames;
  size_t _frameWidth;
  size_t _frameHeight;
  double _lastFrameTime;
  int _fps;
  double _cd;
  bool _playing;
  bool _loop;
};

#endif // ANIM_HPP
