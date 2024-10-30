#include "position.hpp"
#include <cmath>
#include <ostream>

Position::Position(long double x, long double y) : x{x}, y{y} {}

Position::Position() : x{0}, y{0} {}

Position Position::operator+=(const Position& p) { return Position(x + p.x, y + p.y); }

Position Position::operator*(long double scalar) { return Position(x * scalar, y * scalar); }

Position Position::operator/=(long double scalar) { return Position(x / scalar, y / scalar); }

long double Position::distance(Position p1, Position p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

// std::ostream& operator<<(std::ostream& os, const Position& p) {
//     os << "(" << p.x << "," << p.y << ")";
//     return os;
// }

std::string Position::toString() { return "(position)"; }