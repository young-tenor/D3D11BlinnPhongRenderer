#include "pch.h"
#include "Camera.h"

Camera::Camera(HWND hWnd) : hWnd(hWnd)
{
    RECT rc;
    GetClientRect(hWnd, &rc);
    width = rc.right - rc.left;
    height = rc.bottom - rc.top;
}

void Camera::Update()
{
    UpdateDistance();
    UpdatePosition();
}

void Camera::UpdateDistance()
{
    float wheel = ImGui::GetIO().MouseWheel;
    dist -= wheel;
    dist = std::clamp(dist, 3.0f, 10.0f);
}

void Camera::UpdatePosition()
{
    POINT curPos;
    GetCursorPos(&curPos);
    ScreenToClient(hWnd, &curPos);

    static POINT prevPos = curPos;

    bool lbuttonDown = GetAsyncKeyState(VK_LBUTTON) & 0x8000;

    if (lbuttonDown) {
        float dx = (float)(curPos.x - prevPos.x);
        float dy = (float)(curPos.y - prevPos.y);

        yaw += dx * glm::pi<float>() / width;
        pitch += dy * glm::pi<float>() / height;
    }

    prevPos = curPos;

    const float limit = glm::radians(89.0f);
    pitch = std::clamp(pitch, -limit, limit);

    glm::vec4 initialPos = glm::vec4(0.0f, 0.0f, -dist, 1.0f);

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::rotate(rotation, pitch, glm::vec3(1.0f, 0.0f, 0.0f));

    pos = glm::vec3(rotation * initialPos);
}
