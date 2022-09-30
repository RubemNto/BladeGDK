#include "program.hpp"
void Program::run() {
  start();
  update();
}

void Program::start() { core.initBlade(); }

void Program::update() { core.mainLoop(); }
