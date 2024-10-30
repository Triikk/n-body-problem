#pragma once

#include <utility>
#include <ostream>

class Position {
   public:
    long double x;
    long double y;

    Position();
    Position(long double x, long double y);
    Position operator+=(const Position& p);
    Position operator*(long double scalar);
    Position operator/=(long double scalar);
    // friend std::ostream& operator<<(std::ostream& os, const Position& p);
    std::string toString();

    static long double distance(Position p1, Position p2);
};