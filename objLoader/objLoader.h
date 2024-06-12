#ifndef OBJLOADER_H
#define OBJLOADER_H

// Obj loader for VBO and EBO

// VBO : {V.x, V.y, V.z, VT.u, VT.v, VN.x, VN.y, VN.z}
// Vertex size: 8
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
//#include <unordered_map>
#include <map>
#include <utility>

struct Vertex3f {
    float x, y, z;
};
struct VertexData {
    int vertexIndex;
    int textureIndex;
    int normalIndex;
};
struct Face {
    VertexData v1;
    VertexData v2;
    VertexData v3;
};
struct Mesh {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    size_t vertAmount = 0;
};
using uniqVertex_t = std::map<std::vector<int>, unsigned int>;
class ObjLoader {
private:
    Mesh mesh;
    uniqVertex_t uniqVertices;
public:
    ObjLoader();
    Mesh loadObjFile(const std::string& objName);
    void printMesh(Mesh mesh);
private:
    std::vector<VertexData> faceParse(const std::string& faceString);
};

#endif