#include "Object3D.h"

Object3D::Object3D(objectType premitive, glm::vec4 newColor, unsigned int newShaderProgram)
    :
    size(1.0f),
    pos(glm::vec3(0.0f)),
    angles(glm::vec3(0.0f)),
    ambientStr(0.2f),
    trans(glm::mat4(1.0f)),
    shaderProgram(newShaderProgram),
    color(newColor)
{
    createPremitive(premitive);
    initVA();
}

Object3D::~Object3D() {}

void Object3D::createPremitive(objectType premitive) {
    PremitiveGenerator premitiveGenerator;
    mesh = premitiveGenerator.createSphere(1.0f, 100, 100);
}

void Object3D::setSize(float newSize) {
    size = newSize;
}

void Object3D::setPos(glm::vec3 newPos) {
    pos = newPos;
}

void Object3D::setAngles(glm::vec3 newAngles) {
    angles = newAngles;
}

void Object3D::setColor(glm::vec3 newColor) {
    color.x = newColor.x;
    color.y = newColor.y;
    color.z = newColor.z;
}

glm::vec3 Object3D::getColor() {
    return glm::vec3(color.x, color.y, color.z);
}

void Object3D::setOpacity(float opacity) {
    color.w = opacity;
}

float Object3D::getOpacity() {
    return color.w;
}

void Object3D::setAmbient(float ambient) {
    ambientStr = ambient;
}

glm::mat4 Object3D::getModel() {
    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size/10, size/10, size/10));
    return model;
}

glm::vec3 Object3D::getPos() {
    return pos;
}

int Object3D::getVAO() {
    return VAO;
}

mesh_t Object3D::getMesh() {
    return mesh;
}

void Object3D::initVA() {
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
    // normal pointer
    GLuint normAttrib = glGetAttribLocation(shaderProgram, "aNorm");
    glVertexAttribPointer(normAttrib, 3, GL_FLOAT
        , GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(normAttrib);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
