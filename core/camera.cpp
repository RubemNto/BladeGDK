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
  if (_is3D == true) {
    mat = glm::rotate(mat, rotation.x, glm::vec3(1, 0, 0));
    mat = glm::rotate(mat, rotation.y, glm::vec3(0, 1, 0));
  }
  mat = glm::rotate(mat, rotation.z, glm::vec3(0, 0, 1));
  _orientation = mat;
}

void Camera::updateCamera() {
  _view =
      glm::lookAt(getCameraPosition(),
                  getCameraPosition() -
                      glm::normalize(glm::vec3(glm::inverse(_orientation)[2])),
                  _up);
  if (_is3D == true) {
    _projection = glm::perspective(_fov, _aspect, _near, _far);
  } else {
    _projection = glm::ortho(_left, _right, _bottom, _top, _near, _far);
  }
  std::cout << "Projection:" << std::endl;
  std::cout << glm::to_string(_projection) << std::endl;
}

// 3D constructor Camera
Camera::Camera(glm::vec3 position, glm::vec3 orientation, glm::vec3 up,
               float fov, float aspect, float near, float far) {
  _is3D = true;

  _fov = fov;
  _aspect = aspect;
  _up = up;

  _near = near;
  _far = far;
  setCameraPosition(position);
  setCameraOrientation(orientation);
  updateCamera();
}

// 2D constructor Camera
Camera::Camera(glm::vec2 position, float rotation, float left, float right,
               float bottom, float top, float near, float far) {
  _is3D = false;

  _left = left;
  _right = right;
  _bottom = bottom;
  _top = top;

  _near = near;
  _far = far;
  setCameraPosition(glm::vec3(position, 0));
  setCameraOrientation(glm::vec3(0, 0, rotation));
  updateCamera();
}

glm::mat4 Camera::View() { return _view; }
glm::mat4 Camera::Projection() { return _projection; }