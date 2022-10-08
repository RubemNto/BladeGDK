#include "core/camera.hpp"
#include "core/core.hpp"
using namespace BladeGDK;

class Program {
public:
  void run();

private:
  Core core;
  Camera camera;
  void start();
  void update();
};
