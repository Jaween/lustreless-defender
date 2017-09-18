#ifndef ENGINE_H_
#define ENGINE_H_

#include "SDL_gpu.h"

#include "room.h"

class Engine {
 public:
  Engine(const Room& room);
  void run();

 private:
  static const int kScreenWidth;
  static const int kScreenHeight;

  GPU_Target* window;
  Room room;

  bool init();
  void finish();
};

#endif // ENGINE_H_