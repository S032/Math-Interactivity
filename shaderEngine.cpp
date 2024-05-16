#include "shaderEngine.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}  

ShaderEngine::ShaderEngine(int newWinWidth, int newWinHeight, const char* newWinTitle, bool polygonal)
    :
    winWidth(newWinWidth),
    winHeight(newWinHeight),
    winTitle(newWinTitle)
{
    cameraProps_t camProps;
    camProps.FOV = 45.0f;
    camProps.zFar = 100.0f;
    camProps.speed = 0.2f;
    camProps.sensivity = 1.0f;
    camProps.pos = glm::vec3(0.0f, 0.0f, 5.0f);
    camProps.target = glm::vec3(0.0f, 0.0f, 0.0f);
    camProps.winWidth = winWidth;
    camProps.winHeight = winHeight;
    cam = new Camera(camProps);
    initWindow(polygonal);
    initShader();
}

void ShaderEngine::initWindow(bool polygonal) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW\n");
        exit(EXIT_FAILURE);
    }

    win = glfwCreateWindow(winWidth, winHeight, winTitle, NULL, NULL);
    if (win == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(win);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    if (polygonal) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glViewport(0, 0, winWidth, winHeight);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
}

void ShaderEngine::initShader() {
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

std::string ShaderEngine::readGLSLfile(const std::string& fileName) {
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

void ShaderEngine::checkStatus(unsigned int shader, unsigned int checkStatus) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, checkStatus, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" 
                  << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
}

void ShaderEngine::processInput() {
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, true);
    }
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
        cam->move(CAM_FORWARD);
    }
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
        cam->move(CAM_BACKWARD);
    }
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
        cam->move(CAM_RIGHT);
    }
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
        cam->move(CAM_LEFT);
    }
    if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) {
        cam->move(CAM_UP);
    }
    if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) {
        cam->move(CAM_DOWN);
    } 
    if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cam->rotate(CAM_RIGHT);
    }
    if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cam->rotate(CAM_LEFT);
    }
    if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) {
        cam->rotate(CAM_UP);
    }
    if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cam->rotate(CAM_DOWN);
    }
}

void ShaderEngine::display() {
}

void ShaderEngine::start() {
    _3dObject cube("cube.obj", glm::vec3(0.2f, 0.2f, 1.0f), shaderProgram);
    _3dObject lightcube("cube.obj", glm::vec3(1.0f, 1.0f, 1.0f), shaderProgram);
    _3dObject chair("chair.obj", glm::vec3(0.5882f, 0.2941f, 0.0f),  shaderProgram);
    float angleX = 0.0f;
    float angleY = 0.0f;
    float angleZ = 0.0f;
    while(!glfwWindowShouldClose(win))
    {
        processInput();

        //display();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj_view = cam->getProjViewMat();

        glm::vec3 camDir = cam->getCamPos();
        printf("camDir: %.1f %.1f %.1f\n", camDir.x, camDir.y, camDir.z);


        chair.setsize(5.0f);
        chair.setpos(glm::vec3(2.0f + 0.5f, 2.0f - 1.0f, 0.0f));
        chair.draw(proj_view, camDir);

        lightcube.setsize(5.0f);
        lightcube.setpos(glm::vec3(0.0f, 2.0f, 3.5f));
        lightcube.draw(proj_view, camDir);

        cube.setsize(20.0f);
        cube.setpos(glm::vec3(0.0f, 0.0f, 0.0f));
        cube.draw(proj_view, camDir);
        cube.setpos(glm::vec3(2.0f, 0.0f, 0.0f));
        cube.draw(proj_view, camDir);
        cube.setpos(glm::vec3(0.0f, 0.0f, 2.0f));
        cube.draw(proj_view, camDir);
        cube.setpos(glm::vec3(0.0f, 0.0f, 4.0f));
        cube.draw(proj_view, camDir);

        glfwSwapBuffers(win);
        glfwPollEvents();    
    }
    glfwTerminate();
}                   