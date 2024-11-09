#pragma once

#include "position.hpp"

using namespace std;

class Particle {
   public:
    double mass;
    Position position;

   public:
    Particle(double mass, Position position);
    friend ostream& operator<<(ostream& os, const Particle& p);
    bool operator==(const Particle& p);
};