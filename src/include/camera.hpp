#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "constants.hpp"

/* enum denoting which direction camera should move */
enum CameraDirection { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
public:
  Camera(glm::vec3 world_up = CAMERA_DEFAULT_WORLD_UP,
         glm::vec3 position = CAMERA_DEFAULT_POSITION,
         float yaw = CAMERA_DEFAULT_YAW, float pitch = CAMERA_DEFAULT_PITCH);

  /* accessor methods for keyboard/moust sensitivity */
  float getMoveSensitivity() const;
  float getViewSensitivity() const;

  /* accesor methods for pitch/yaw */
  float getPitch() const;
  float getYaw() const;
  float getFOV() const;

  /* accesor methods for camera axes and position */
  glm::vec3 getRight() const;
  glm::vec3 getUp() const;
  glm::vec3 getFront() const;
  glm::vec3 getPosition() const;

  /* compute the camera's current view matrix */
  glm::mat4 getViewMatrix() const;

  /* assignment methods for move/view sensitivity */
  void setMoveSensitivity(float sensitivity);
  void setViewSensitivity(float sensitivity);

  /* update camera position given direction and time delta */
  void updatePosition(CameraDirection cd, float delta_time);

  /* update view direction based on mouse input */
  void updateView(float yaw_offset, float pitch_offset,
                  GLboolean constrain_pitch = true);

  /* update FOV based on mouse scroll */
  void updateFOV(float offset);

private:
  float _move_sensitivity;  /* scales how fast the camera position moves */
  float _view_sensitivity;  /* scales how fast the camera view moves */

  float _pitch;             /* vertical tilt in degrees */
  float _yaw;               /* horizontal tilt in degrees */
  float _fov;               /* field of view in degrees */

  glm::vec3 _right;         /* x-axis of camera space */
  glm::vec3 _up;            /* y-axis of camera space */
  glm::vec3 _front;         /* z-axis of camera space */

  glm::vec3 _position;      /* camera position in world space */
  glm::vec3 _world_up;      /* up direction of world space */

  void _updateAxes();       /* updates camera vectors */
};

#endif /* ifndef CAMERA_HPP */
