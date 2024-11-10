#pragma once

#include <utility>
#include <ostream>

using namespace std;

class Vector {
   public:
    long double x;
    long double y;

    Vector();
    Vector(long double x, long double y);
    Vector operator+=(const Vector& v);
    Vector operator*(long double scalar);
    Vector operator/=(long double scalar);
    bool operator==(const Vector& v);
    bool operator!=(const Vector& v);
    friend ostream& operator<<(ostream& os, const Vector& v);
    string toString();
};