#include "Engine3D.h"
#include "Object3D.h"
#include "Render.h"
#include <cmath>

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


Engine3D::Engine3D(int newWinWidth, int newWinHeight, const char* newWinTitle, bool polygonal)
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
    dotHandler = new DotHandler();
    initWindow(polygonal);
}

void Engine3D::initWindow(bool polygonal) {
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

void Engine3D::processInput() {
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
    //mouse button
    bool isButtonPressed = (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    if (isButtonPressed && !wasButtonPressed) {
        intersectHandler(cam->getCamPos());
    }
    wasButtonPressed = isButtonPressed;
}

void Engine3D::intersectHandler(glm::vec3 rayOrigin) {
    float normxpos = (2 * mousexpos) / currentWinWidth - 1;
    float normypos = (2 * mouseypos) / currentWinHeight - 1;

    glm::vec4 point = glm::vec4(normxpos, -normypos, -1, 1);
    point = glm::inverse(proj) * point;
    point.w = 0;
    point = glm::inverse(view) * point;
    glm::vec3 rayDir = glm::vec3(point.x, point.y, point.z);
    rayDir = glm::normalize(rayDir);


    float t = 1;
    float step = 0.1f;
    glm::vec3 ray = rayOrigin + (rayDir * t);
    for (float t = 1; t <= 200; t += step) {
        ray = rayOrigin + (rayDir * t);
        if (ray.y <= 0) break;
    }

    dot_t newdot;
    newdot.pos = glm::vec3(ray.x, 0, ray.z);
    printf("x(%.4f)y(%.4f)z(%.4f)\n", normxpos, normypos, rayDir.z);

    dotHandler->spawnDot(newdot);
}

void Engine3D::primitiveDrawer(glm::mat4 proj_view, glm::vec3 camPos, glm::vec3 lightPos) {
    std::vector<dot_t> dots = dotHandler->getDotsArr();

    //Object3D sphere(objectType::SPHERE, glm::vec4(0.2f, 0.2f, 1.0f, 1.0f),  shaderProgram);

    for (dot_t dot : dots) {
        //sphere.setPos(dot.pos);
        //sphere.draw(proj_view, camPos, lightPos);
    }
}

void Engine3D::start() {
    unsigned int shaderProgram{};
    Render render(shaderProgram);
    Object3D lightcube(objectType::CUBE, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), shaderProgram);
    Object3D field(objectType::CUBE, glm::vec4(1.0f, 1.0f, 1.0f, 0.2f), shaderProgram);
    objectType type = objectType::SPHERE;
    Object3D sphere(type, glm::vec4(0.2f, 0.2f, 1.0f, 1.0f),  shaderProgram);
    while(!glfwWindowShouldClose(win))
    {
        processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        proj = cam->getProjMat();
        view = cam->getViewMat();
        glm::mat4 proj_view = proj * view;
        glm::vec3 camPos = cam->getCamPos();

        lightcube.setPos(glm::vec3(glm::cos(glfwGetTime()*2) * 4, glm::sin(glfwGetTime()*2) * 4, glm::sin(glfwGetTime()*2) * 4));
        glm::vec3 lightPos = lightcube.getPos();

        render.setProjView(proj_view);
        render.setAmbientStrength(1.0f);
        render.setCameraPosition(camPos);
        render.setLightPosition(lightPos);
        render.drawFigure(lightcube);
        render.setAmbientStrength(0.1f);

        //field.setsize(200.0f);

        sphere.setSize(5.0f);
        render.drawFigure(sphere);

        glfwSwapBuffers(win);
        glfwPollEvents();    
    }
    glfwTerminate();
}                   
