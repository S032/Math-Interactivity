#ifndef DOTHANDLER_H
#define DOTHANDLER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <list>
#include <vector>

struct dot_t {
    std::string title;
    glm::vec3 pos;
    glm::vec3 normal;
};

struct polygon_t {
    std::vector<dot_t> vertices;
};

class DotHandler {
private:
    const float dotRadius = 12.0f;
    std::vector<dot_t> dots;
public:
    DotHandler();
    std::vector<dot_t> getDotsArr();
    void spawnDot(dot_t newDot);
private:
};

#endif
