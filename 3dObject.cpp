#include "3dObject.h"

_3dObject::_3dObject(std::string objName, std::string texName, unsigned int newShaderProgram)
    :
    size(1.0f),
    pos(glm::vec3(0.0f)),
    angles(glm::vec3(0.0f)),
    aChannel(1.0f),
    ambientStr(0.2f),
    trans(glm::mat4(1.0f)),
    shaderProgram(newShaderProgram),
    color(glm::vec3(1.0f)),
    colormix(0.0f)
{
    mesh = objLoader.loadObjFile(objName);
    initVA();
    TextureLoad(texName);
}

_3dObject::_3dObject(std::string objName, glm::vec3 newcolor, unsigned int newShaderProgram)
    :
    size(1.0f),
    pos(glm::vec3(0.0f)),
    angles(glm::vec3(0.0f)),
    aChannel(1.0f),
    ambientStr(0.2f),
    trans(glm::mat4(1.0f)),
    shaderProgram(newShaderProgram),
    color(newcolor),
    colormix(1.0f)
{
    mesh = objLoader.loadObjFile(objName);
    initVA();
    //TextureLoad("box.jpg");
}

_3dObject::~_3dObject() {}

void _3dObject::setsize(float newSize) {
    size = newSize;
}

void _3dObject::setpos(glm::vec3 newPos) {
    pos = newPos;
}

void _3dObject::setangles(glm::vec3 newAngles) {
    angles = newAngles;
}

void _3dObject::setOpacity(float opacity) {
    aChannel = opacity;
}

void _3dObject::setAmbient(float ambient) {
    ambientStr = ambient;
}

glm::vec3 _3dObject::getPos() {
    return pos;
}

void _3dObject::draw(glm::mat4 proj_view, glm::vec3 camPos, glm::vec3 lightPos) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size/10, size/10, size/10));
    glm::mat4 mvp = proj_view * model;

    //fragmentShader uniforms
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    GLuint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(glm::vec4(1.0f)));
    GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    GLuint camPosLoc = glGetUniformLocation(shaderProgram, "camPos");
    glUniform3fv(camPosLoc, 1, glm::value_ptr(camPos));
    GLuint aChanelLoc = glGetUniformLocation(shaderProgram, "aChanel");
    glUniform1f(aChanelLoc, aChannel);
    GLuint ambientStrLoc = glGetUniformLocation(shaderProgram, "ambietStrength");
    glUniform1f(ambientStrLoc, ambientStr);

    //vertexShader uniforms
    GLuint mvpLoc = glGetUniformLocation(shaderProgram, "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);   
    glBindVertexArray(0);
}

void _3dObject::initVA() {
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float),
        mesh.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int),
        mesh.indices.data(), GL_STATIC_DRAW);
    // vertex pointer
    GLuint posAttrib = glGetAttribLocation(shaderProgram, "aPos"); 
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT
        , GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(posAttrib);
    // texture pointer
    GLuint texAttrib = glGetAttribLocation(shaderProgram, "aTex");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT
        , GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(texAttrib);
    // normal pointer
    GLuint normAttrib = glGetAttribLocation(shaderProgram, "aNorm");
    glVertexAttribPointer(normAttrib, 3, GL_FLOAT
        , GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(normAttrib);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void _3dObject::TextureLoad(std::string texName) {
    glActiveTexture(GL_TEXTURE0);
    std::string texPath = "resources/" + texName;
    texture = SOIL_load_OGL_texture(texPath.c_str(),
    SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    if (!texture) {
        std::cerr << "Failed to load texture" << std::endl;
        exit(EXIT_FAILURE);
    }
    glUniform1i(glGetUniformLocation(shaderProgram, "Box"), texture);

    // Настройка параметров текстуры
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
