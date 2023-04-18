#include <glm/gtc/matrix_transform.hpp>

#include "include/camera.hpp"

Camera::Camera(glm::vec3 world_up, glm::vec3 position, float yaw, float pitch):
      _front(CAMERA_DEFAULT_FRONT),
      _move_sensitivity(CAMERA_DEFAULT_MOVEMENT_SPEED),
      _view_sensitivity(CAMERA_DEFAULT_VIEW_SENSITIVITY),
      _fov(CAMERA_DEFAULT_FOV) {
  _world_up = world_up;
  _position = position;
  _yaw = yaw;
  _pitch = pitch;
  _updateCamera();
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(_position, _position + _front, _up);
}

float Camera::getFOV() const {
  return _fov;
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

  if (constrain_pitch) _pitch = glm::clamp(_pitch, -89.0f, 89.0f);

  _updateCamera();
}

void Camera::updateFOV(float offset) {
  _fov = glm::clamp(_fov - offset, 1.0f, 120.0f);
}

void Camera::_updateCamera() {
  _front = glm::normalize(glm::vec3(
    cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
    sin(glm::radians(_pitch)),
    sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))));

  _right = glm::normalize(glm::cross(_front, _world_up));
  _up    = glm::normalize(glm::cross(_right, _front));
}
