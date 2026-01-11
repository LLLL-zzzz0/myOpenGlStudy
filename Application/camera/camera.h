#pragma once

#include "../../glframework/core.h"
#include <glm/detail/type_float.hpp>

class CameraControl;
class GameCameraControl;
class TrackBallCameraControl;

class Camera
{
public:
    Camera();
    ~Camera();

    glm::mat4 getViewMatrix();
    virtual glm::mat4 getProjectionMatrix();
    virtual void scale(float fDeltaScale);
	glm::vec3 getPosition() { return m_vec3Position; }
	void setPosition(glm::vec3 vec3Position) {  m_vec3Position = vec3Position; }
	glm::vec3 getUp() { return m_vec3Up; }
	void setUp(glm::vec3 vec3Up) { m_vec3Up = vec3Up; }
    glm::vec3 getRight() { return m_vec3Right; }
	void setRight(glm::vec3 vec3Right) { m_vec3Right = vec3Right; }
	virtual float getNear() { return 0.0f; }
	virtual float getFar() { return 0.0f; }
protected:
    glm::vec3 m_vec3Position{ 0.0f, 0.0f, 5.0f };
    glm::vec3 m_vec3Up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_vec3Right{ 1.0f, 0.0f, 0.0f };
private:
    friend class CameraControl;
    friend class GameCameraControl;
    friend class TrackBallCameraControl;

};