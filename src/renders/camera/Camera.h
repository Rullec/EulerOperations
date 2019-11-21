//
// Created by Hanke on 2018/11/5.
// Revised by Xudong on 2019/11/21.

#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:
    Camera();

    const glm::vec3 &getPos() const;

    const glm::vec3 &getUp() const;

    const glm::vec3 &getTar() const;

    void setPos(const glm::vec3 &pos);

    void setUp(const glm::vec3 &up);

    void setTar(const glm::vec3 &tar);

    void moveLeft(float camera_speed=0.1);

    void moveRight(float camera_speed=0.1);

    void moveForward(float camera_speed=0.1);

    void moveBackward(float camera_speed=0.1);

    void mouseMove(float mouse_x, float moues_y);

    glm::mat4 ViewMatrix();

private:
    glm::vec3 pos;
    glm::vec3 up;
    glm::vec3 tar;
    glm::vec3 camera_front;

    float pitch;
    float yaw;
    float last_x;
    float last_y;
    bool firstMouse;
};
