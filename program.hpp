#include "core/core.hpp"
using namespace BladeGDK;

class Program {
public:
  void run();

private:
  Core core;
  void start();
  void update();
};
