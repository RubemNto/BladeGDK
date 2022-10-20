#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace BladeGDK {
class Camera {
private:
  glm::mat4 _view;
  glm::mat4 _projection;

  glm::mat4 _position;
  glm::mat4 _orientation;
  glm::vec3 _eulerOrientation;

  bool _is3D;

  /*3D Camera Data*/
  float _fov;
  float _aspect;
  float _near;
  float _far;

  /*2D Camera Data*/

public:
  glm::vec3 getCameraPosition();

  glm::vec3 getCameraEulerOrientation();

  void setCameraPosition(glm::vec3 position);

  void setCameraOrientation(glm::vec3 rotation);

  void updateCamera();

  // 3D constructor Camera
  Camera(glm::vec3 position, glm::vec3 orientation, glm::vec3 up, float fov,
         float aspect, float near, float far);
  glm::mat4 View();
  glm::mat4 Projection();
};
} // namespace BladeGDK
