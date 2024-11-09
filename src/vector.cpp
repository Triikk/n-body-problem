#include "vector.hpp"
#include <ostream>

Vector::Vector(long double x, long double y) : x{x}, y{y} {}

Vector::Vector() : x{0}, y{0} {}

Vector Vector::operator+=(const Vector& v) { return Vector(x + v.x, y + v.y); }

Vector Vector::operator*(long double scalar) { return Vector(x * scalar, y * scalar); }

Vector Vector::operator/=(long double scalar) { return Vector(x / scalar, y / scalar); }

bool Vector::operator==(const Vector& v) { return x == v.x && y == v.y; }

ostream& operator<<(ostream& os, const Vector& v) {
    os << "(" << v.x << "," << v.y << ")";
    return os;
}

string Vector::toString() { return "(vector)"; }