#include "vector.hpp"
#include <ostream>

Vector::Vector(long double x, long double y) : x{x}, y{y} {}

Vector::Vector() : x{0}, y{0} {}

Vector Vector::operator+=(const Vector& v) {
    x += v.x;
    y += v.y;
    return *this;
}

Vector Vector::operator*(long double scalar) { return Vector(x * scalar, y * scalar); }

Vector Vector::operator/=(long double scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

bool Vector::operator==(const Vector& v) { return abs(x - v.x) < 1e-9 && abs(y - v.y) < 1e-9; }

bool Vector::operator!=(const Vector& v) { return !(*this == v); }

ostream& operator<<(ostream& os, const Vector& v) {
    os << "(" << v.x << "," << v.y << ")";
    return os;
}

string Vector::toString() { return "(vector)"; }