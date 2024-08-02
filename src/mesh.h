#ifndef MESH_H
#define MESH_H

#include <vector>

struct mesh_t {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif
