#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CAM_FORWARD 0
#define CAM_BACKWARD 1
#define CAM_RIGHT 2
#define CAM_LEFT 3
#define CAM_UP 4
#define CAM_DOWN 5

struct camRot_t {
    float pitch;
    float yaw;
    float roll;
};

struct cameraProps_t {
    float FOV, zFar;
    float speed, sensivity;
    glm::vec3 pos, target;
    float winWidth, winHeight;
};

class Camera {
private:
    float FOV;
    float zFar;
    float speed;
    float sensivity;
    camRot_t camRot;
    float winWidth;
    float winHeight;
    glm::vec3 pos;
    glm::vec3 target;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    glm::mat4 proj;
    glm::mat4 view;
public:
    Camera(cameraProps_t camProps);
    glm::mat4 getProjViewMat();
    glm::mat4 getProjMat();
    glm::mat4 getViewMat();
    glm::vec3 getCamDir();
    glm::vec3 getCamPos();
    void move(int direction);
    void rotate(int direction);
    void updateWinAspect(float newWinWidth, float newWinHeight);
private:
    void updateViewMat();
};

#endif