#include "DotHandler.h"

DotHandler::DotHandler() {

}

std::vector<dot_t> DotHandler::getDotsArr() {
    return dots;
}

void DotHandler::spawnDot(dot_t newDot) {
    dots.push_back(newDot);
}

