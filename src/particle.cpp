#include <ostream>
#include "position.hpp"
#include "particle.hpp"

Particle::Particle() {};

Particle::Particle(double mass, Position position)
    : mass{mass}, position{position}, velocity{Velocity()}, acceleration{Acceleration()} {}

ostream& operator<<(ostream& os, const Particle& p) {
    os << "Particle<mass=" << p.mass << ",position=" << p.position << ">";
    return os;
}

string Particle::toString() { return "Particle<mass=" + to_string(mass) + ",position=" + position.toString() + ">"; }

bool Particle::operator==(const Particle& p) { return mass == p.mass && position == p.position; }

void Particle::computeDisplacement(float delta) {
    velocity.x = velocity.x + acceleration.x * delta;
    velocity.y = velocity.y + acceleration.y * delta;

    position.x = position.x + velocity.x * delta;
    position.y = position.y + velocity.y * delta;
}