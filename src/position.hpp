#pragma once

#include <utility>
#include <ostream>

using namespace std;

class Position {
   public:
    long double x;
    long double y;

    Position();
    Position(long double x, long double y);
    Position operator+=(const Position& p);
    Position operator*(long double scalar);
    Position operator/=(long double scalar);
    friend ostream& operator<<(ostream& os, const Position& p);
    string toString();

    static long double distance(Position p1, Position p2);
};