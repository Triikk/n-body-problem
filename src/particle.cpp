#include "particle.hpp"
#include <ostream>

Particle::Particle(double mass, Position position) : mass{mass}, position{position} {}

// std::ostream& operator<<(std::ostream& os, const Particle*& p) {
//     os << "Particle<mass=" << p->mass << ",position=" << p->position << ">";
//     return os;
// }

std::string Particle::toString() {
    return "Particle<mass=" + std::to_string(mass) + ",position=" + position.toString() + ">";
}