#include "shaderEngine.h"
#include <string>

int main(int argc, char* argv[]) {
    int winWidht = 800;
    int winHeight = 600;
    bool polygonal = false;
    if (argc == 3) {
        winWidht = std::stoi(argv[1]);
        winHeight = std::stoi(argv[2]);
    }
    else if (argc == 4) {
        winWidht = std::stoi(argv[1]);
        winHeight = std::stoi(argv[2]);
        if (strcmp( argv[3], "-p") == 0)
            polygonal = true;
    }

    ShaderEngine engine(winWidht, winHeight, "ShaderTest", polygonal);
    engine.start();
}