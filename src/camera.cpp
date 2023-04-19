#include <glm/gtc/matrix_transform.hpp>

#include "include/camera.hpp"
#include "include/constants.hpp"

Camera::Camera(glm::vec3 world_up, glm::vec3 position, float yaw, float pitch):
      _front(CAMERA_DEFAULT_FRONT),
      _move_sensitivity(CAMERA_DEFAULT_MOVE_SENSITIVITY),
      _view_sensitivity(CAMERA_DEFAULT_VIEW_SENSITIVITY),
      _fov(CAMERA_DEFAULT_FOV) {
  _world_up = world_up;
  _position = position;
  _yaw = yaw;
  _pitch = pitch;
  _updateAxes();
}

float Camera::getMoveSensitivity() const {
  return _move_sensitivity;
}

float Camera::getViewSensitivity() const {
  return _view_sensitivity;
}

float Camera::getPitch() const {
  return _pitch;
}

float Camera::getYaw() const {
  return _yaw;
}

float Camera::getFOV() const {
  return _fov;
}

glm::vec3 Camera::getRight() const {
  return _right;
}

glm::vec3 Camera::getUp() const {
  return _up;
}

glm::vec3 Camera::getFront() const {
  return _front;
}

glm::vec3 Camera::getPosition() const {
  return _position;
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(_position, _position + _front, _up);
}

void Camera::setMoveSensitivity(float sensitivity) {
  _move_sensitivity = sensitivity;
}

void Camera::setViewSensitivity(float sensitivity) {
  _view_sensitivity = sensitivity;
}

void Camera::updatePosition(CameraDirection cd, float delta_time) {
  float speed = _move_sensitivity * delta_time;
  if (cd == FORWARD)  _position += _front * speed;
  if (cd == BACKWARD) _position -= _front * speed;
  if (cd == LEFT)     _position -= _right * speed;
  if (cd == RIGHT)    _position += _right * speed;
}

void Camera::updateView(float yaw_offset, float pitch_offset,
                        GLboolean constrain_pitch) {
  _yaw   += _view_sensitivity * yaw_offset;
  _pitch += _view_sensitivity * pitch_offset;

  if (constrain_pitch)
    _pitch = glm::clamp(_pitch, CAMERA_MIN_PITCH_DEGREES,
                                CAMERA_MAX_PITCH_DEGREES);

  _updateAxes();
}

void Camera::updateFOV(float offset) {
  _fov = glm::clamp(_fov - offset, CAMERA_MIN_FOV_DEGREES,
                                   CAMERA_MAX_FOV_DEGREES);
}

void Camera::_updateAxes() {
  _front = glm::normalize(glm::vec3(
    cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
    sin(glm::radians(_pitch)),
    sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))));

  _right = glm::normalize(glm::cross(_front, _world_up));
  _up    = glm::normalize(glm::cross(_right, _front));
}
