#pragma once

#include "position.hpp"

class Particle {
   public:
    double mass;
    Position position;

   public:
    Particle();
    Particle(double mass, Position position);
};