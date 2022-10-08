#include "program.hpp"

void Program::run() {
  start();
  update();
}

void Program::start() {
  camera = Camera(
      glm::vec3(0, 0, 0),
      glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f)),
      glm::perspective(glm::radians(60.0f), (800 / 600.0f), 0.1f, 10.0f));

  core._vertices = {
      {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
      {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
      {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

      {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
      {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
      {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

      {{-0.5f, -0.5f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      {{0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
      {{0.5f, 0.5f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
      {{-0.5f, 0.5f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
  };

  core._indices = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8};
  core.initBlade();
}

void Program::update() { core.mainLoop(camera); }
