#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    Camera(HWND hWnd);

    const UINT GetWidth() const { return width; }
    const UINT GetHeight() const { return height; }
    const glm::vec3 &GetPos() const { return pos; }
    const glm::mat4 &GetView() const { return view; }

    void Update();

private:
    void UpdateDistance();
    void UpdatePosition();

private:
    HWND hWnd;
    UINT width = 1280;
    UINT height = 720;

    float dist = 3.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, -3.0f);

    glm::vec3 at = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view = glm::mat4(1.0f);
};