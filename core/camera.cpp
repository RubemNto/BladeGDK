#include "camera.hpp"
#include "coreComponents.hpp"

using namespace BladeGDK;

glm::vec3 Camera::getCameraPosition() {
  glm::vec4 tPosition = _position * glm::vec4(0, 0, 0, 1);
  glm::vec3 position = glm::vec3(tPosition.x, tPosition.y, tPosition.z);
  return position;
}

glm::vec3 Camera::getCameraEulerOrientation() { return _eulerOrientation; }

void Camera::setCameraPosition(glm::vec3 position) {
  _position = glm::translate(glm::mat4(1), position);
  updateCamera();
}

void Camera::setCameraOrientation(glm::vec3 rotation) {
  _eulerOrientation = rotation;
  glm::mat4 mat = glm::mat4(1);
  mat = glm::rotate(mat, rotation.x, glm::vec3(1, 0, 0));
  mat = glm::rotate(mat, rotation.y, glm::vec3(0, 1, 0));
  mat = glm::rotate(mat, rotation.z, glm::vec3(0, 0, 1));
  _orientation = mat;
}

void Camera::updateCamera() {
  _view =
      glm::lookAt(getCameraPosition(),
                  getCameraPosition() -
                      glm::normalize(glm::vec3(glm::inverse(_orientation)[2])),
                  glm::vec3(0, -1, 0));
  if (_is3D == true) {
    _projection = glm::perspective(_fov, _aspect, _near, _far);
  } else {
    _projection = glm::ortho(0, (int)_swapChainExtent.width, 0,
                             (int)_swapChainExtent.height);
  }
}

// 3D constructor Camera
Camera::Camera(glm::vec3 position, glm::vec3 orientation, glm::vec3 up,
               float fov, float aspect, float near, float far) {
  _is3D = true;
  _fov = fov;
  _aspect = aspect;
  _near = near;
  _far = far;
  setCameraPosition(position);
  setCameraOrientation(orientation);
  updateCamera();
}
glm::mat4 Camera::View() { return _view; }
glm::mat4 Camera::Projection() { return _projection; }