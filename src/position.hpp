#pragma once

#include <utility>

class Position {
   public:
    long double x;
    long double y;

    Position();
    Position(long double x, long double y);
    Position operator+=(const Position& p);
    Position operator*(long double scalar);
    Position operator/=(long double scalar);

    static long double distance(Position p1, Position p2);
};