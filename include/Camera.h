#ifndef CAMERA_H
#define CAMERA_H

#include <stdbool.h>

#include "./Window.h"
#include "./Space.h"

typedef struct
{
    Mat4 matrix;
    Vec3 position;
    Vec3 front;
    float yaw;
    float pitch;
    float fov;
    float speed;
    float sensitivity;
    float aspectRatio;
    bool locked;
} Camera;

Camera camera_create(float fov, float speed, float sensitivity, float aspectRatio);
void   camera_updateYaw(Camera* camera, float yaw);
void   camera_updatePitch(Camera* camera, float pitch);
void   camera_updateFov(Camera* camera, float fov);
void   camera_updateSpeed(Camera* camera, float speed);
void   camera_updateSensitivity(Camera* camera, float sensitivity);
void   camera_updateAspectRatio(Camera* camera, float aspectRatio);

void camera_lock(Camera* camera);
void camera_unlock(Camera* camera);
void camera_recomputePosition(Camera* camera, Window* window);
void camera_recomputeRotation(Camera* camera, Window* window);
void camera_recomputeMatrix(Camera* camera);

#endif // CAMERA_H
