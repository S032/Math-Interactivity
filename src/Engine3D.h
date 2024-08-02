#ifndef SHADER_ENGINE_H
#define SHADER_ENGINE_H

#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>
#include "Camera.h"
#include "DotHandler.h"
#include "Render.h"
#include "Object3D.h"
#include <GLFW/glfw3.h>

class Engine3D {
private:
    GLFWwindow *win;
    Camera *cam;
    DotHandler *dotHandler;
    int winWidth;
    int winHeight;
    const char* winTitle;
    glm::mat4 proj;
    glm::mat4 view;
    //flags
    bool wasButtonPressed = false;
public:
    Engine3D(int newWinWidth, int newWinHeight, const char* newWinTitle, bool polygonal);
    void start();
private:
    void initWindow(bool polygonal);
    void updateWinAspect(int newWinWidth, int newWinHeight);
    void processInput();
    void intersectHandler(glm::vec3 rayOrigin);
    void primitiveDrawer(glm::mat4 proj_view, glm::vec3 camPos, glm::vec3 lightPos);
};

#endif
