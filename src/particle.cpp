#include "particle.hpp"
#include <ostream>

Particle::Particle(double mass, Position position) : mass{mass}, position{position} {}

ostream& operator<<(ostream& os, const Particle& p) {
    os << "Particle<mass=" << p.mass << ",position=" << p.position << ">";
    return os;
}

string Particle::toString() { return "Particle<mass=" + to_string(mass) + ",position=" + position.toString() + ">"; }