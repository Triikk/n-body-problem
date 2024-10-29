#include "particle.hpp"

Particle::Particle(double mass, Position position) {
    this->mass = mass;
    this->position = position;
}

Particle::Particle() {
    this->mass = 0;
    this->position = Position();
}