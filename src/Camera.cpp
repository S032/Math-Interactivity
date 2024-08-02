#include "Camera.h"

Camera::Camera(cameraProps_t camProps)
    :
    FOV(camProps.FOV),
    zFar(camProps.zFar),
    speed(camProps.speed),
    sensivity(camProps.sensivity),
    winWidth(camProps.winWidth),
    winHeight(camProps.winHeight),
    pos(camProps.pos),
    target(camProps.target)
{
    proj = glm::perspective(glm::radians(FOV), winWidth/winHeight, 0.1f, zFar);
    camRot = {0.0f, 0.0f, 0.0f};
    front.x = glm::cos(glm::radians(camRot.yaw));
    front.z = glm::sin(glm::radians(camRot.yaw));
    front.y = glm::sin(glm::radians(camRot.pitch));
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::normalize(glm::cross(up, front));
    up = glm::normalize(glm::cross(front, right));
    view = glm::lookAt(pos, pos + front, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::move(int direction) {
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::normalize(glm::cross(up, front));
    up = glm::normalize(glm::cross(front, right));
    switch (direction)
    {
    case CAM_FORWARD:
        pos += front * speed;
        break;
    case CAM_BACKWARD:
        pos -= front * speed;
        break;
    case CAM_LEFT:
        pos -= right * speed;
        break;
    case CAM_RIGHT:
        pos += right * speed;
        break;
    case CAM_UP:
        pos += glm::vec3(0.0f, 1.0f, 0.0f) * speed;
        break;
    case CAM_DOWN:
        pos -= glm::vec3(0.0f, 1.0f, 0.0f) * speed;
        break;
    default:
        break;
    }
    updateViewMat();
}

void Camera::rotate(int direction) {
    switch (direction)
    {
    case CAM_RIGHT:
        camRot.yaw -= sensivity;
        break;
    case CAM_LEFT:
        camRot.yaw += sensivity;
        break;
    case CAM_UP:
        camRot.pitch += sensivity;
        break;
    case CAM_DOWN:
        camRot.pitch -= sensivity;
        break;
    default:
        break;
    }
    if(camRot.pitch > 88.0f) {
        camRot.pitch = 88.0f;
    }
    if(camRot.pitch < -88.0f) {
        camRot.pitch = -88.0f;
    }

    front.x = glm::cos(glm::radians(camRot.yaw)) * cos(glm::radians(camRot.pitch));
    front.y = glm::sin(glm::radians(camRot.pitch));
    front.z = glm::sin(glm::radians(camRot.yaw)) * cos(glm::radians(camRot.pitch));
    updateViewMat();
}

void Camera::updateWinAspect(float newWinWidth, float newWinHeight) {
    winWidth = newWinWidth;
    winHeight = newWinHeight;
}

glm::vec3 Camera::getCamDir() {
    return front;
}

glm::vec3 Camera::getCamPos() {
    return pos;
}

glm::mat4 Camera::getProjViewMat() {
    return proj * view;
}

glm::mat4 Camera::getProjMat() {
    return proj;
}

glm::mat4 Camera::getViewMat() {
    return view;
}

void Camera::updateViewMat() {
    view = glm::lookAt(pos, pos + front, glm::vec3(0.0f, 1.0f, 0.0f));
}
