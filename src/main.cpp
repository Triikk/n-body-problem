#include <iostream>
#include <set>
#include <cassert>
#include <cmath>
#include <algorithm>

#include "quadtree.hpp"
#include "view.hpp"

#define G 6.67430e+2

using namespace std;

vector<Particle> particles;

void clearParticles() { particles.clear(); }

bool isPositionAvailable(Position p1) {
    for (auto particle : particles) {
        if (p1 == particle.position) return false;
    }
    return true;
}

// generate randomly particles in all different positions
void initializeRandomParticles(int count, int length) {
    for (int i = 0; i < count; i++) {
        Particle p;
        do {
            p = Particle(1, Position(((float)rand() / RAND_MAX) * length, ((float)rand() / RAND_MAX) * length));
        } while (!isPositionAvailable(p.position));
        particles.push_back(p);
    }
    /*cerr << "InitParticles:" << endl;
    for (auto &p : particles) {
        cerr << "\tparticle " << p << " at " << &p << endl;
    }*/
}

// generate particles sequentially
void initializeSequentialParticles(double max) {
    for (int x = 0; x < max; x++) {
        for (int y = 0; y < max; y++) {
            Particle p = Particle(1, Position(x, y));
            cerr << "\t" << p << " at " << &p << ")" << endl;
            particles.push_back(p);
        }
    }
}

// store in the target particle acceleration field the sum of all accelerations due to gravitational interaction with
// the other particles. if the particles are in the same position the interaction is irrelevant
void computeSingleForce(Particle &target, Particle &actor) {
    // assert(!(target->position == actor->position));
    if (!(target.position == actor.position)) {
        float distance_module = Position::distance(target.position, actor.position);  // d
        float dx = -(target.position.x - actor.position.x);                           // d.x
        float dy = -(target.position.y - actor.position.y);                           // d.y

        float acc_module = G * actor.mass / pow(distance_module, 2.0);  // a
        float acc_x = acc_module * (dx / distance_module);              // a.x
        float acc_y = acc_module * (dy / distance_module);              // a.y

        target.acceleration.x += acc_x;
        target.acceleration.y += acc_y;
    }
}

/**
 * Compute the net force of particle `p` with particles on node `n` and below.
 */
void recursiveNetForce(Node *n, Particle &p, double theta) {
    if (n->children.empty() && !n->particle) {  //  empty node without children
        return;
    } else if (n->particle) {  //  leaf node therefore compute interaction
        computeSingleForce(p, *n->particle);
        // p->mass = n->particle->mass * 2; // fix with formula
    } else if ((n->length / Position::distance(n->centerOfMass, p.position)) < theta) {
        //  grouping node which can be used for approximating the underlying tree (its subtree 8D)
        // maybe add !n->particle to condition
        Particle actor = Particle(n->totalMass, n->centerOfMass);
        computeSingleForce(p, actor);
    } else {
        // grouping node which cant be used for approximating: therefore whe recursively check its
        // children
        for (auto &child : n->children) {
            recursiveNetForce(child, p, theta);
        }
    }
}

/**
 * Compute the net force of particle `p`.
 * @param qt quadtree
 * @param p particle
 * @param theta threshold for the approximation
 */
void computeNetForce(Quadtree &qt, Particle &p, double theta) { recursiveNetForce(qt.root, p, theta); }

bool isOutside(Quadtree &qt, Particle &p) {
    return p.position.x < 0.0 || p.position.x > qt.length || p.position.y < 0.0 || p.position.y > qt.length;
}

/**
 * Update the state of each particle after one round of interaction.
 * @param qt quadtree
 * @param theta threshold for the approximation
 * @param delta time step
 * @param particles vector of particles
 */
void updateParticles(Quadtree &qt, double theta, double delta) {
    for (auto &p : qt.particles) {
        p.acceleration = Acceleration(0, 0);
    }
    for (auto &p : qt.particles) {
        computeNetForce(qt, p, theta);
    }
    // remove particles outside the quadtree
    qt.particles.erase(
        std::remove_if(qt.particles.begin(), qt.particles.end(), [&](Particle &p) { return isOutside(qt, p); }),
        qt.particles.end());
    for (auto &p : qt.particles) {
        p.computeDisplacement(delta);
    }
    assert(!qt.particles.empty());
}

int main() {
    int length = 800;
    int count = 50;
    double delta = 0.01, theta = 0.5;
    initializeRandomParticles(count, length);

    Quadtree qt = Quadtree(length, particles);
    View view = View(qt, length);

    while (true) {
        qt.build();
        qt.computeApproximationValues();
        updateParticles(qt, theta, delta);
        qt.clean();
        view.update();
    }
}
