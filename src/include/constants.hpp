#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <glm/glm.hpp>

/* default window settings */
#define WINDOW_TITLE  "fmarch"
#define WINDOW_DEFAULT_WIDTH  800
#define WINDOW_DEFAULT_HEIGHT 600

/* default camera settings */
#define CAMERA_DEFAULT_MOVE_SENSITIVITY  2.5f
#define CAMERA_DEFAULT_VIEW_SENSITIVITY  0.1f
#define CAMERA_DEFAULT_YAW             -90.0f
#define CAMERA_DEFAULT_PITCH             0.0f
#define CAMERA_DEFAULT_FOV              45.0f
#define CAMERA_MIN_PITCH_DEGREES       -89.0f
#define CAMERA_MAX_PITCH_DEGREES        89.0f
#define CAMERA_MIN_FOV_DEGREES           1.0f
#define CAMERA_MAX_FOV_DEGREES         120.0f

const glm::vec3 CAMERA_DEFAULT_POSITION { 0.0f,  0.0f,  0.0f };
const glm::vec3 CAMERA_DEFAULT_WORLD_UP { 0.0f,  1.0f,  0.0f };
const glm::vec3 CAMERA_DEFAULT_FRONT    { 0.0f,  0.0f, -1.0f };

/* fullscreen quad (two triangles) for the vertex shader since raymarching is
 * done entirely in fragent shader */
const float FS_QUAD[] = {
  -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
  -1.0f,  1.0f, 0.0f,
   1.0f,  1.0f, 0.0f
};

/* define opengl function params for fullscreen quad */
#define FS_QUAD_VERTEX_ATTRIB_INDEX 0
#define FS_QUAD_DRAW_ARRAY_PARAMS GL_TRIANGLE_STRIP, 0, 4
#define FS_QUAD_VERTEX_ATTRIB_PARAMS FS_QUAD_VERTEX_ATTRIB_INDEX, \
    3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0

#endif /* ifndef CONSTANTS_H */
