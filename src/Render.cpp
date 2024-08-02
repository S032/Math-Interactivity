#include "Render.h"
#include "Object3D.h"
#include <string>

Render::Render(unsigned int &_shaderProgram) {
    initShaderProgram();
    _shaderProgram = shaderProgram;
}

Render::~Render() {

}

void Render::initShaderProgram() {
    GLuint res = glewInit();
    if (GLEW_OK != res)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
        exit(EXIT_FAILURE);
    }

    //creating vertex shader
    std::string tempstr = readGLSLfile("vertexShader.glsl");
    const char* vertexShaderSource = tempstr.c_str();
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkStatus(vertexShader, GL_COMPILE_STATUS);

    //creating fragment shader;
    tempstr = readGLSLfile("fragmentShader.glsl");
    const char* fragmentShaderSource = tempstr.c_str();
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkStatus(fragmentShader, GL_COMPILE_STATUS);
    //assembing Shader Programm
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "Fragment");
    glLinkProgram(shaderProgram);
    checkStatus(shaderProgram, GL_LINK_STATUS);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

std::string Render::readGLSLfile(const std::string& fileName) {
    std::string filePath = "shaders/" + fileName;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        fprintf(stderr, "Failed to read %s\n", filePath.c_str());
        return "";
    }
    std::string shaderSource((std::istreambuf_iterator<char>(file))
        , std::istreambuf_iterator<char>());
    
    shaderSource += "\0";
    return shaderSource;
}

void Render::checkStatus(unsigned int shader, unsigned int checkStatus) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, checkStatus, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" 
                  << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Render::setProjView(const glm::mat4& newProjView) {
    projView = newProjView;
}

void Render::setAmbientStrength(const float &newAmbientStr) {
    ambientStrength = newAmbientStr; 
}

void Render::setCameraPosition(const glm::vec3 &newCameraPos) {
    cameraPosition = newCameraPos;
}

void Render::setLightPosition(const glm::vec3 &newLightPos) {
    lightPosition = newLightPos;
}

void Render::drawFigure(Object3D &object) {
    glUseProgram(shaderProgram);
    glBindVertexArray(object.getVAO());

    glm::mat4 model = object.getModel();
    glm::mat4 mvp = projView * model;
    glm::vec3 objectColor = object.getColor();
    float objectOpacity = object.getOpacity();
    mesh_t mesh = object.getMesh();

//fragmentShader uniforms
    glm::vec3 color3f = glm::vec3(objectColor.x, objectColor.y, objectColor.z);
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
    glUniform3fv(colorLoc, 1, glm::value_ptr(color3f));
    GLuint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(glm::vec4(1.0f)));
    GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPosition));
    GLuint camPosLoc = glGetUniformLocation(shaderProgram, "camPos");
    glUniform3fv(camPosLoc, 1, glm::value_ptr(cameraPosition));
    GLuint aChanelLoc = glGetUniformLocation(shaderProgram, "aChanel");
    glUniform1f(aChanelLoc, objectOpacity);
    GLuint ambientStrLoc = glGetUniformLocation(shaderProgram, "ambietStrength");
    glUniform1f(ambientStrLoc, ambientStrength);

    //vertexShader uniforms
    GLuint mvpLoc = glGetUniformLocation(shaderProgram, "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);   
    glBindVertexArray(0);
}


