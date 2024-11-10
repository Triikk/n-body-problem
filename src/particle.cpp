#include <ostream>
#include "position.hpp"
#include "particle.hpp"

Particle::Particle() {};

Particle::Particle(double mass, Position position)
    : mass{mass}, position{position}, velocity{Velocity()}, acceleration{Acceleration()} {}

ostream& operator<<(ostream& os, const Particle& p) {
    os << "<Particle<mass=" << p.mass << ",pos=" << p.position << ",vel=" << p.velocity << ",acc=" << p.acceleration
       << ">";
    return os;
}

bool Particle::operator==(const Particle& p) { return mass == p.mass && position == p.position; }

void Particle::computeDisplacement(float delta) {
    velocity.x += acceleration.x * delta;
    velocity.y += acceleration.y * delta;

    position.x += velocity.x * delta;
    position.y += velocity.y * delta;
}