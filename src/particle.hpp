#pragma once

#include <vector>

#include "position.hpp"
#include "velocity.hpp"
#include "acceleration.hpp"

using namespace std;

class Particle {
   public:
    double mass;
    double radius;
    Position position;
    Velocity velocity;
    Velocity collision_velocity;
    Acceleration acceleration;
    vector<Particle&> colliding_particles;

   public:
    Particle();
    Particle(double mass, Position position);
    friend ostream& operator<<(ostream& os, const Particle& p);
    bool operator==(const Particle& p);
    void computeDisplacement(float delta);
    void computeCollisionDisplacement(float delta);
    void computeSingleForce(Particle& actor);
    void addCollidingParticle(Particle& particle);
    void computeCollisions();

    static bool doCollide(Particle& p1, Particle& p2);
};