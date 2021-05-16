#pragma once

#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace GUI {

class Camera : public Object {
public:

    Camera() = default;

    void SetWindowSize(float w, float h)
    {
        width = w;
        height = h;
    }

    void SetClipping(float near, float far)
    {
        nearClipping = near;
        farClipping = far;
    }

    glm::mat4 GetProjectionMat() const { return glm::perspective(FOV, width / height, nearClipping, farClipping); }
    glm::mat4 GetViewMat() const { return glm::mat4_cast(glm::conjugate(GetRotation())) * glm::translate(-GetPosition()); }

private:
    float width = 1.f;
    float height = 1.f;
    float nearClipping = 0.1f;
    float farClipping = 100.f;
    float FOV = glm::radians(45.0f);
    float speed = 3.0f;
    float mouseSpeed = 0.005f;
};

} // namespace GUI
