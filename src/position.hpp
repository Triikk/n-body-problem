#pragma once

#include <utility>

class Position {
   public:
    long double x;
    long double y;

    Position(long double x, long double y);

    static long double distance(Position p1, Position p2);
};