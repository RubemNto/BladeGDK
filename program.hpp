#include "core/camera.hpp"
#include "core/core.hpp"
using namespace BladeGDK;

static Camera *camera;

class Program {
public:
  void run();

private:
  Core core;
  void start();
  void update();
};
