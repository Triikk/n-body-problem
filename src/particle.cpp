#include <ostream>
#include <iostream>
#include <cassert>
#include <cmath>

#include "position.hpp"
#include "particle.hpp"
#include "physics.hpp"

Particle::Particle() {};

Particle::Particle(double mass, Position position)
    : mass{mass}, position{position}, velocity{Velocity()}, acceleration{Acceleration()} {
    radius = pow(mass, 1.0 / 3) / 1e3;
}

Particle::Particle(double mass, Position position, Velocity velocity, double radius)
    : mass{mass}, position{position}, velocity{velocity}, radius{radius}, acceleration{Acceleration()} {}

ostream& operator<<(ostream& os, const Particle& p) {
    os << "<Particle<mass=" << p.mass << ",pos=" << p.position << ",vel=" << p.velocity << ",acc=" << p.acceleration
       << ",radius=" << p.radius << ">";
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

void Particle::addCollidingParticle(Particle* particle) {
    assert(!(*this == *particle));
    colliding_particles.push_back(particle);
}

bool Particle::doCollide(Particle& p1, Particle& p2) {
    return Position::distance(p1.position, p2.position) < (p1.radius + p2.radius);
}

void Particle::computeCollisions() {
    assert(acceleration == Acceleration(0, 0));
    collision_velocity = Velocity(0, 0);
    for (auto& p : colliding_particles) {
        collision_velocity += velocity - (position - p->position) * ((2 * p->mass) / (mass + p->mass)) *
                                             (((velocity - p->velocity) * (position - p->position)) /
                                              (pow(Position::distance(position, p->position), 2.0)));
    }
    // cout << "coll_vel: " << collision_velocity << endl;
    // cout << "pos: " << position << endl;
}

void Particle::computeCollisionDisplacement(float delta) {
    if (collision_velocity != Velocity(0, 0)) {
        velocity = collision_velocity;
    }

    computeDisplacement(delta);
}