#include "pch.h"
#include "Camera.h"

Camera::Camera(HWND hWnd) : hWnd(hWnd) {
}

void Camera::Update() {
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

    glm::vec4 initialPos = glm::vec4(0.0f, 0.0f, -radius, 1.0f);

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::rotate(rotation, pitch, glm::vec3(1.0f, 0.0f, 0.0f));

    pos = glm::vec3(rotation * initialPos);

    view = glm::lookAtLH(pos, at, up);
}