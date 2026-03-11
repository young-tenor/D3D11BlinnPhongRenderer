#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    Camera(HWND h_wnd);

    void Update();

public:
    HWND hWnd;
    float width = 1280.0f;
    float height = 720.0f;

    float radius = 3.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, -3.0f);

    glm::vec3 at = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view = glm::mat4(1.0f);
};