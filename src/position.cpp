#include "position.hpp"
#include <cmath>

Position::Position(long double x, long double y) : x{x}, y{y} {}

long double Position::distance(Position p1, Position p2) {
    return std::sqrtl(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}