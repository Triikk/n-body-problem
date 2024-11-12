#include <cmath>
#include "position.hpp"

bool Position::operator<=(const Position& p) { return x <= p.x && y <= p.y; }

long double Position::distance(Position p1, Position p2) { return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)); }