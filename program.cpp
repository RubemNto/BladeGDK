#include "program.hpp"

void Program::run() {
  start();
  update();
}

void Program::start() {
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

  // 3D camera
  camera = new Camera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                      glm::vec3(0, 1, 0), glm::radians(60.0f),
                      _swapChainExtent.width / (float)_swapChainExtent.height,
                      0.1f, 1000.0f);

  // 2D Camera
  //  camera = new Camera(glm::vec2(0, 0), 0.0f, 0.0f, _swapChainExtent.width,
  //  0.0f,
  //                      _swapChainExtent.height, 0.0f, 100.0f);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
    camera->setCameraPosition(camera->getCameraPosition() +
                              glm::vec3(-1, 0, 0));
  }
  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
    camera->setCameraPosition(camera->getCameraPosition() + glm::vec3(1, 0, 0));
  }
}

void Program::update() {
  glfwSetKeyCallback(_window, key_callback);
  core.mainLoop(camera);
}
