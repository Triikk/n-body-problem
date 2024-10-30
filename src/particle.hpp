#pragma once

#include "position.hpp"

class Particle {
   public:
    double mass;
    Position position;

   public:
    Particle(double mass, Position position);
    // friend std::ostream& operator<<(std::ostream& os, const Particle*& p);
    std::string toString();
};