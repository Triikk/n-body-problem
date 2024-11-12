#pragma once

#include "vector.hpp"

class Position : public Vector {
   public:
    using Vector::Vector;
    bool operator<=(const Position& p);

    static long double distance(Position p1, Position p2);
};