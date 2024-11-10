#include <ostream>
#include <cassert>
#include <cmath>

#include "position.hpp"
#include "particle.hpp"

#define G 6.674304e-8

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

void Particle::computeSingleForce(Particle& actor) {
    // assert(!(position == actor.position));
    if (position != actor.position) {
        float distance_module = Position::distance(position, actor.position);  // d
        float dx = actor.position.x - position.x;                              // d.x
        float dy = actor.position.y - position.y;                              // d.y

        float acc_module = G * actor.mass / pow(distance_module, 2.0);  // a
        float acc_x = acc_module * (dx / distance_module);              // a.x
        float acc_y = acc_module * (dy / distance_module);              // a.y

        acceleration.x += acc_x;
        acceleration.y += acc_y;
    }
}