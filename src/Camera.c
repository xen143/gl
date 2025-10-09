#include "../include/Camera.h"

Camera camera_create(float fov, float speed, float sensitivity, float aspectRatio)
{
    Camera camera =
    {
        .position = vec3(4.f, 2.f, 4.f),
        .front = vec3(0.f, 0.f, -1.f),
        .yaw = -90.f,
        .pitch = 0.f,
        .fov = fov,
        .speed = speed,
        .sensitivity = sensitivity,
        .aspectRatio = aspectRatio,
        .locked = false,
    };
    mat4_load_identity(&camera.matrix);
    return camera;
}

void camera_updateYaw(Camera* camera, float yaw)
{
    camera->yaw = yaw;
}

void camera_updatePitch(Camera* camera, float pitch)
{
    camera->pitch = pitch;
}

void camera_updateFov(Camera* camera, float fov)
{
    camera->fov = fov;
}

void camera_updateSpeed(Camera* camera, float speed)
{
    camera->speed = speed;
}

void camera_updateSensitivity(Camera* camera, float sensitivity)
{
    camera->sensitivity = sensitivity;
}

void camera_updateAspectRatio(Camera* camera, float aspectRatio)
{
    camera->aspectRatio = aspectRatio;
}

void camera_lock(Camera* camera)
{
    camera->locked = true;
}

void camera_unlock(Camera* camera)
{
    camera->locked = false;
}

void camera_recomputePosition(Camera* camera, Window* window)
{
    if (!camera->locked) return;

    Vec3 movementVec = vec3_normalize(window_getMovementVec(window));
    Vec3 movementDirection = vec3(0.f, 0.f, 0.f);
    Vec3 right = vec3_normalize(vec3_cross(camera->front, vec3(0.f, 1.f, 0.f)));
    
    movementDirection = vec3_add(movementDirection, vec3_scale(camera->front, movementVec.z));
    movementDirection = vec3_add(movementDirection, vec3_scale(right, movementVec.x));
    movementDirection = vec3_add(movementDirection, vec3_scale(vec3(0, 1.f, 0.f), movementVec.y));
    
    movementDirection = vec3_normalize(movementDirection);
    vec3_add_inplace(&camera->position, vec3_scale(movementDirection, camera->speed * window->deltaTime));
}

void camera_recomputeRotation(Camera* camera, Window* window)
{
    if (!camera->locked) return;

    double mouseX;
    double mouseY;
    glfwGetCursorPos(window->glfwWindow, &mouseX, &mouseY);

    int windowWidth;
    int windowHeight;
    glfwGetFramebufferSize(window->glfwWindow, &windowWidth, &windowHeight);

    const float deltaX = (float)(mouseX - (float)windowWidth / 2.f) * camera->sensitivity;
    const float deltaY = (float)(mouseY - (float)windowHeight / 2.f) * camera->sensitivity;

    camera->yaw += deltaX * camera->sensitivity;
    camera->pitch -= deltaY * camera->sensitivity;

    if (camera->pitch > 89.f)
    {
        camera->pitch = 89.f;
    }
    else if (camera->pitch < -89.f)
    {
        camera->pitch = -89.f;
    }
    camera->yaw = fmod(camera->yaw, 360.f);

    Vec3 front = vec3(0.f, 0.f, 0.f);
    front.x = cosf(radians(camera->yaw)) * cosf(radians(camera->pitch));
    front.y = sinf(radians(camera->pitch));
    front.z = sinf(radians(camera->yaw)) * cosf(radians(camera->pitch));
    camera->front = vec3_normalize(front);
}

void camera_recomputeMatrix(Camera* camera)
{
    Mat4* view = mat4_lookAt(
        camera->position,
        vec3_add(camera->position, camera->front),
        vec3(0.f, 1.f, 0.f)
    );
    Mat4* projection = mat4_perspective(
        camera->fov,
        camera->aspectRatio,
        0.01f,
        100.f
    );

    mat4_load_identity(&camera->matrix);
    mat4_multiply_many_inplace(&camera->matrix, 2, view, projection);

    free(view);
    free(projection);
}
