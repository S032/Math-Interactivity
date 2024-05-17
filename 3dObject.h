#ifndef _3DOBJECT_H
#define _3DOBJECT_H

#include "objLoader/objLoader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
#include <iostream>

class _3dObject {
private:
    ObjLoader objLoader;
    Mesh mesh;
    float size;
    glm::vec3 pos;
    glm::vec3 angles;
    float aChannel;
    float ambientStr;
    glm::mat4 trans;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    unsigned int shaderProgram;
    //texture
    unsigned int texture;
    glm::vec3 color;
    float colormix;
public:
    _3dObject(std::string objName, std::string texName, unsigned int newShaderProgram);
    _3dObject(std::string objName, glm::vec3 newcolor, unsigned int newShaderProgram);
    ~_3dObject();
    void setsize(float newSize);
    void setpos(glm::vec3 newPos);
    void setangles(glm::vec3 newAngles);
    void setOpacity(float opacity);
    void setAmbient(float ambient);
    glm::vec3 getPos();
    void draw(glm::mat4 proj_view, glm::vec3 camPos, glm::vec3 lightPos);
private:
    void initVA();
    void TextureLoad(std::string texName);
};
#endif