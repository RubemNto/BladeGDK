#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace BladeGDK {
class Camera {
private:
  glm::mat4 _view;
  glm::mat4 _projection;

public:
  glm::vec3 _direction = glm::vec3(0);

  Camera() {}
  Camera(glm::mat4 view, glm::mat4 projection) {
    _view = view;
    _projection = projection;
  }
  glm::mat4 View() { return _view; }
  glm::mat4 Projection() { return _projection; }
};
} // namespace BladeGDK
