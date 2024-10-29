#include "position.hpp"
#include <cmath>

Position::Position(long double x, long double y) {
    this->x = x;
    this->y = y;
}

Position::Position() {
    this->x = 0;
    this->y = 0;
}

long double Position::distance(Position p1, Position p2) {
    return std::sqrtl(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}