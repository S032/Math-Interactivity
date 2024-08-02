#ifndef _3DOBJECT_H
#define _3DOBJECT_H

#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "PremitiveGenerator.h"

// NeedToDO: transfer draw function into Render class

enum class objectType {
    TRIANGLE,
    SPHERE,
    PLAIN,
    CUBE
};

class Object3D {
private:
    mesh_t mesh;
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
    glm::vec4 color;
public:
    Object3D(objectType primitive, glm::vec4 newcolor, unsigned int newShaderProgram);
    ~Object3D();
    void setSize(float newSize);
    void setPos(glm::vec3 newPos);
    void setAngles(glm::vec3 newAngles);
    void setColor(glm::vec3 newColor);
    glm::vec3 getColor();
    void setOpacity(float opacity);
    float getOpacity();
    void setAmbient(float ambient);
    glm::mat4 getModel();
    glm::vec3 getPos();
    int getVAO();
    mesh_t getMesh();
    void draw(glm::mat4 proj_view, glm::vec3 camPos, glm::vec3 lightPos);
private:
    void createPremitive(objectType);
    void initVA();
};
#endif
