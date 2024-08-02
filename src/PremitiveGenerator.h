#ifndef PREMITIVEGENERATOR_H
#define PREMITIVEGENERATOR_H

#include "DotHandler.h"
#include <cmath>
#include "mesh.h"

class PremitiveGenerator {
private:
    DotHandler *dotHandler;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    unsigned int shaderProgram;
public:
    PremitiveGenerator();
    mesh_t createSphere(float radius, uint32_t segLatitude, glm::uint32 segLongitude);
private:
    std::vector<polygon_t> generatePolygons(std::vector<dot_t> &dots);
    glm::vec3 calcNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
};

#endif
