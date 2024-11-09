#pragma once

#include "vector.hpp"

class Position : public Vector {
   public:
    using Vector::Vector;

    static long double distance(Position p1, Position p2);
};