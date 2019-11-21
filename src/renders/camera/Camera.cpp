//
// Created by Hanke on 2018/11/5.
// Revised by Xudong on 2019/11/21.

#include "Camera.h"

Camera::Camera() :
        pos(glm::vec3(0.0f, 0.f, 1.0f)),
        tar(glm::vec3(0.0f, 0.f, 0.0f)),
        up(glm::vec3(0.0f, 1.0f, 0.0f)),
        last_x(0),
        last_y(0),
        pitch(0),
        yaw(0),
        firstMouse(true)
{
    camera_front = glm::normalize(tar - pos);
}

const glm::vec3 &Camera::getPos() const {
    return pos;
}

void Camera::setPos(const glm::vec3 &pos) {
    Camera::pos = pos;
}

const glm::vec3 &Camera::getUp() const {
    return up;
}

void Camera::setUp(const glm::vec3 &up) {
    Camera::up = up;
}

glm::mat4 Camera::ViewMatrix() {
    return glm::lookAt(Camera::pos, Camera::pos + Camera::camera_front, Camera::up);
}

const glm::vec3 &Camera::getTar() const {
    return tar;
}

void Camera::setTar(const glm::vec3 &tar) {
    Camera::tar = tar;
}

void Camera::setFirstPos(float x, float y)
{
    last_x = x;
    last_y = y;
}

void Camera::moveLeft(float camera_speed) {
    glm::vec3 move = glm::cross(camera_front, Camera::up);
    move = glm::normalize(move) * camera_speed;
    Camera::pos -= move;
}

void Camera::moveRight(float camera_speed) {
    glm::vec3 move = glm::cross(camera_front, Camera::up);
    move = -glm::normalize(move) * camera_speed;
    Camera::pos -= move;
}

void Camera::moveForward(float camera_speed) {
    Camera::pos += glm::normalize(camera_front) * camera_speed;
}

void Camera::moveBackward(float camera_speed) {
    Camera::pos -= glm::normalize(camera_front) * camera_speed;
}

void Camera::moveUp(float camera_speed)
{
    Camera::pos += glm::normalize(up) * camera_speed;
}

void Camera::moveDown(float camera_speed)
{
    Camera::pos -= glm::normalize(up) * camera_speed;
}

void Camera::mouseMove(float mouse_x, float mouse_y) {

    if (firstMouse)
    {
        last_x = mouse_x;
        last_y = mouse_y;
        firstMouse = false;
    }
    float x_offset = mouse_x - last_x;
    float y_offset = -mouse_y + last_y;

    last_x = mouse_x;
    last_y = mouse_y;

    float sensitivity = 0.1f;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw   += x_offset;
    pitch += y_offset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = -cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    camera_front = glm::normalize(front);
}
