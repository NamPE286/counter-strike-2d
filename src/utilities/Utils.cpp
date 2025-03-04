#include "Utils.hpp"

#include <numbers>
#include <cmath>

float Utils::getAngle(int x1, int y1, int x2, int y2) {
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    float angle = std::atan2(deltaY, deltaX);

    return angle;
}