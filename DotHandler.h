#ifndef DOTHANDLER_H
#define DOTHANDLER_H

#include "objLoader/objLoader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

struct dot_t {
    std::string title;
    std::string primitive;
    glm::vec3 pos;
    glm::vec4 color;
};

class DotHandler {
private:
    const float dotRadius = 12.0f;
    std::vector<dot_t> dots;
public:
    DotHandler();
    std::vector<dot_t> getDotsArr();
private:
    void createDot(dot_t newdot);


};

#endif