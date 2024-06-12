#include "shaderEngine.h"

float currentWinWidth = 0;
float currentWinHeight = 0;
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    currentWinWidth = width;
    currentWinHeight = height;
    glViewport(0, 0, width, height);
}

//mouse
float mousexpos = 0;
float mouseypos = 0;
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    mousexpos = xpos;
    mouseypos = ypos;
}


ShaderEngine::ShaderEngine(int newWinWidth, int newWinHeight, const char* newWinTitle, bool polygonal)
    :
    winWidth(newWinWidth),
    winHeight(newWinHeight),
    winTitle(newWinTitle)
{
    cameraProps_t camProps;
    camProps.FOV = 90.0f;
    camProps.zFar = 100.0f;
    camProps.speed = 0.05f;
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
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    if (polygonal) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glViewport(0, 0, winWidth, winHeight);

    glfwSetCursorPosCallback(win, mouse_callback);
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
    if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            std::cout << "Левая кнопка мыши нажата" << std::endl;
    }
}

void ShaderEngine::intersectHandler(glm::mat4 proj, glm::mat4 view, glm::vec3 rayOrigin, _3dObject &sphere) {
    float normxpos = (2 * mousexpos) / currentWinWidth - 1;
    float normypos = (2 * mouseypos) / currentWinHeight - 1;
    glm::vec4 point = glm::vec4(normxpos, -normypos, -1, 1);
    point = glm::inverse(proj) * point;
    point.w = 0;
    point = glm::inverse(view) * point;
    glm::vec3 rayDir = glm::vec3(point.x, point.y, point.z);
    rayDir = glm::normalize(rayDir);

    float t = 1;
    float step = 0.5f;
    glm::vec3 ray = rayOrigin + (rayDir * t);
    for (float t = 1; t <= 200; t += step) {
        ray = rayOrigin + (rayDir * t);
        if (ray.y <= 0) break;
    }
    sphere.setpos(glm::vec3(ray.x, 0, ray.z));



    printf("after: x(%.4f) y(%.4f), z(%.4f)\n",
        rayDir.x, rayDir.y, rayDir.z);
}

void ShaderEngine::start() {
    _3dObject lightcube("cube.obj", glm::vec4(1.0f, 1.0f, 1.0f, 0), shaderProgram);
    _3dObject field("field.obj", glm::vec4(1.0f, 1.0f, 1.0f, 0.2f), shaderProgram);
    _3dObject sphere("sphere.obj", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f),  shaderProgram);
    while(!glfwWindowShouldClose(win))
    {
        processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = cam->getProjMat();
        glm::mat4 view = cam->getViewMat();

        glm::mat4 proj_view = proj * view;

        glm::vec3 camPos = cam->getCamPos();

        lightcube.setsize(5.0f);
        lightcube.setAmbient(1.0f);
        lightcube.setpos(glm::vec3(glm::cos(glfwGetTime()*2) * 4, glm::sin(glfwGetTime()*2) * 4, glm::sin(glfwGetTime()*2) * 4));
        glm::vec3 lightPos = lightcube.getPos();
        lightcube.draw(proj_view, camPos, lightPos);

        field.setsize(200.0f);
        field.draw(proj_view, camPos, lightPos);

        sphere.setsize(5.0f);
        sphere.draw(proj_view, camPos, lightPos);

        intersectHandler(proj, view, camPos, sphere);

        glfwSwapBuffers(win);
        glfwPollEvents();    
    }
    glfwTerminate();
}                   