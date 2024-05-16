#ifndef SHADER_ENGINE_H
#define SHADER_ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <SOIL/SOIL.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "3dObject.h"
#include "camera.h"

class ShaderEngine {
private:
    GLFWwindow *win;
    Camera *cam;
    int winWidth;
    int winHeight;
    const char* winTitle;
    unsigned int shaderProgram;
public:
    ShaderEngine(int newWinWidth, int newWinHeight, const char* newWinTitle, bool polygonal);
    void start();
private:
    void initWindow(bool polygonal);
    void initShader();
    void updateWinAspect(int newWinWidth, int newWinHeight);
    std::string readGLSLfile(const std::string& fileName);
    void checkStatus(unsigned int shader, unsigned int checkStatus);
    void processInput();
    void display();
};

#endif