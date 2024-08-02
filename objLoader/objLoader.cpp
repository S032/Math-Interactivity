#include "objLoader.h"

ObjLoader::ObjLoader() {
    
}

Mesh ObjLoader::loadObjFile(const std::string& objName) {
    std::vector<Vertex3f> vertices;
    std::vector<Vertex3f> uvs;
    std::vector<Vertex3f> normals;
    std::vector<Face> faces;
    std::ifstream objFile("resources/" + objName);
    if (!objFile.is_open()) {
        std::cerr << "couldn't load resources/" 
                  + objName << std::endl;
        exit(EXIT_FAILURE);
    }
    //reading obj
    std::string line;
    while (std::getline(objFile, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "v") {
            Vertex3f vertex;
            iss >> vertex.x >> vertex.y >> vertex.z; 
            vertices.push_back(vertex);
        }
        else if (type == "vt") {
            Vertex3f uv;
            iss >> uv.x >> uv.y >> uv.z; 
            uvs.push_back(uv);
        }
        else if (type == "vn") {
            Vertex3f normal;
            iss >> normal.x >> normal.y >> normal.z; 
            normals.push_back(normal);
        }
        else if (type == "f") {
            std::string faceline = {line.begin() + 1, line.end()};
            //std::cout << "faceline: " << faceline << std::endl;
            std::vector<VertexData> verticesz = faceParse(faceline);
            for (VertexData vertex : verticesz) {
                // is there such vertex within hashmap
                std::vector<int> indecites = {vertex.vertexIndex, vertex.textureIndex, vertex.normalIndex};
                //printf("V: %d/%d/%d ", vertex.vertexIndex, vertex.textureIndex, vertex.normalIndex); 
                if (uniqVertices.find(indecites)
                    == uniqVertices.end())
                {
                    //printf("N: %ld\n", mesh.vertAmount);
                    uniqVertices[indecites] = mesh.vertAmount;
                    mesh.indices.push_back(mesh.vertAmount);
                    mesh.vertAmount++;
                    mesh.vertices.push_back(vertices[vertex.vertexIndex - 1].x);
                    mesh.vertices.push_back(vertices[vertex.vertexIndex - 1].y);
                    mesh.vertices.push_back(vertices[vertex.vertexIndex - 1].z);
                    mesh.vertices.push_back(uvs[vertex.textureIndex - 1].x);
                    mesh.vertices.push_back(uvs[vertex.textureIndex - 1].y);
                    mesh.vertices.push_back(normals[vertex.normalIndex - 1].x);
                    mesh.vertices.push_back(normals[vertex.normalIndex - 1].y);
                    mesh.vertices.push_back(normals[vertex.normalIndex - 1].z);
                }
                else {
                    mesh.indices.push_back(uniqVertices[indecites]);
                    //printf("N: %d\n", uniqVertices[indecites]);
                }
            }
        }
    }
    objFile.close();
    return mesh;
}

std::vector<VertexData> ObjLoader::faceParse(const std::string& faceString) {
    std::vector<VertexData> vertices;

    std::istringstream iss(faceString);
    std::string token;
    while (std::getline(iss, token, ' ')) {
        std::istringstream tokenStream(token);
        std::string vertexData;
        while (std::getline(tokenStream, vertexData, '/')) {
            VertexData vertex;
            if (!vertexData.empty()) {
            //std::cout << "VertexData: " << vertexData << std::endl;
            vertex.vertexIndex = std::stoi(vertexData);
            }
            std::getline(tokenStream, vertexData, '/');
            if (!vertexData.empty()) {
            //std::cout << "2VertexData: " << vertexData << std::endl;
                vertex.textureIndex = std::stoi(vertexData);
            }
            std::getline(tokenStream, vertexData, '/');
            if (!vertexData.empty()) {
            //std::cout << "3VertexData: " << vertexData << std::endl;
                vertex.normalIndex = std::stoi(vertexData);
            }
            vertices.push_back(vertex);
        }
    }

    return vertices;
}


void ObjLoader::printMesh(Mesh mesh) {
    mesh = mesh;
    /*
    printf("vertices:\n");
    for (Vertex3f vertex : mesh.vertices) {
        printf("v %.6f %.6f %.6f\n", vertex.x, vertex.y, vertex.z);
    }
    printf("texture coords:\n");
    for (Vertex3f uv : mesh.uvs) {
        printf("vt %.6f %.6f %.6f\n", uv.x, uv.y, uv.z);
    }
    printf("normals:\n");
    for (Vertex3f normal : mesh.normals) {
        printf("vn %.6f %.6f %.6f\n", normal.x, normal.y, normal.z);
    }
    printf("faces:\n");
    printf("size %ld\n", mesh.faces.size());
    for (Face face : mesh.faces) {
        printf("f %d/%d/%d ", face.v1.vertexIndex
                            , face.v1.textureIndex
                            , face.v1.normalIndex);
        printf("%d/%d/%d "  , face.v2.vertexIndex
                            , face.v2.textureIndex
                            , face.v2.normalIndex);
        printf("%d/%d/%d\n" , face.v3.vertexIndex
                            , face.v3.textureIndex
                            , face.v3.normalIndex);
    }
    */ 
}
