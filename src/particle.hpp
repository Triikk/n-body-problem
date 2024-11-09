#pragma once

#include "position.hpp"
#include "velocity.hpp"
#include "acceleration.hpp"

using namespace std;

class Particle {
   public:
    double mass;
    Position position;
    Velocity velocity;
    Acceleration acceleration;

   public:
    Particle();
    Particle(double mass, Position position);
    friend ostream& operator<<(ostream& os, const Particle& p);
    string toString();
    bool operator==(const Particle& p);
    void computeDisplacement(float delta);
};