#pragma once

#include "../../glframework/core.h"
#include <glm/detail/type_float.hpp>

class Camera
{
public:
    Camera();
    ~Camera();

    glm::mat4 getViewMatrix();
    virtual glm::mat4 getProjectionMatrix();
    virtual void scale(float fDeltaScale);

    glm::vec3 m_vec3Position{ 0.0f, 0.0f, 10.0f };
    glm::vec3 m_vec3Up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_vec3Right{ 1.0f, 0.0f, 0.0f };

};