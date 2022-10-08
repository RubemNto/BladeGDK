#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace BladeGDK {
class Camera {
private:
  glm::vec3 _position;
  glm::mat4 _view;
  glm::mat4 _projection;

public:
  Camera() {}
  Camera(glm::vec3 position, glm::mat4 view, glm::mat4 projection) {
    _position = position;
    _view = view;
    _projection = projection;
  }

  glm::vec3 Position() { return _position; }
  glm::mat4 View() { return _view; }
  glm::mat4 Projection() { return _projection; }
};
} // namespace BladeGDK
