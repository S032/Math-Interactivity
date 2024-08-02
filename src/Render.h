#ifndef RENDER_H
#define RENDER_H

#include <fstream>
#include <GL/glew.h>
#include <GL/glu.h>
#include "Object3D.h"

class Render {
private:
    unsigned int shaderProgram;
    glm::mat4 projView;
    float ambientStrength;
    glm::vec3 cameraPosition;
    glm::vec3 lightPosition;
public:
    Render(unsigned int &_shaderProgram);
    ~Render();
    void drawFigure(Object3D &object);
    void setProjView(const glm::mat4 &newProjView);
    void setAmbientStrength(const float &newAmbientStr);
    void setCameraPosition(const glm::vec3 &newCameraPos);
    void setLightPosition(const glm::vec3 &newLightPos);
private:
    void initShaderProgram();
    std::string readGLSLfile(const std::string& fileName);
    void checkStatus(unsigned int shader, unsigned int checkStat);
};
#endif


