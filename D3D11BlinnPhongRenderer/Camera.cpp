#include "pch.h"
#include "Camera.h"

Camera::Camera(HWND hWnd) : hWnd(hWnd) {
}

void Camera::Update() {
    POINT curPos;
    GetCursorPos(&curPos);
    ScreenToClient(hWnd, &curPos);

    static POINT prevPos = curPos;

    bool rightButtonDown = GetAsyncKeyState(VK_RBUTTON) & 0x8000;

    if (rightButtonDown) {
        float dx = (float)(curPos.x - prevPos.x);
        float dy = (float)(curPos.y - prevPos.y);

        yaw += -dx * glm::pi<float>() / width;
        pitch += dy * glm::pi<float>() / height;
    }

    prevPos = curPos;

    const float limit = glm::radians(89.0f);
    pitch = std::clamp(pitch, -limit, limit);

    pos.x = radius * cos(pitch) * sin(yaw);
    pos.y = radius * sin(pitch);
    pos.z = -radius * cos(pitch) * cos(yaw);

    view = glm::lookAtLH(pos, at, up);
}