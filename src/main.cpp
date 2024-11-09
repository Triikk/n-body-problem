#include <iostream>
#include <set>
#include <cassert>
#include <cmath>

#include "quadtree.hpp"

#define G 6.67430e-11

using namespace std;

vector<Particle> particles;

bool isOutside(Quadtree *qt, Particle *p) {
    return p->position.x < 0 || p->position.x > qt->length || p->position.y < 0 ||
           p->position.y > qt->length;  // 0 is qt origin
}
/*
void clearParticles() {
    cout << "clearing particles..." << endl;
    for (auto it = particles.begin(); it != particles.end(); it++) {
        Particle *p = *it;
        delete p;
    }
    particles.clear();
    cout << "particles cleared" << endl;
}*/

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
    /*cout << "InitParticles:" << endl;
    for (auto &p : particles) {
        cout << "\tparticle " << p << " at " << &p << endl;
    }*/
}

// generate particles sequentially
void initializeSequentialParticles(double max) {
    for (int x = 0; x < max; x++) {
        for (int y = 0; y < max; y++) {
            Particle p = Particle(1, Position(x, y));
            cout << "\t" << p << " at " << &p << ")" << endl;
            particles.push_back(p);
        }
    }
}

// store in the target particle acceleration field the sum of all accelerations due to gravitational interaction with
// the other particles. if the particles are in the same position the interaction is irrelevant
void computeInteraction(Particle *target, Particle *actor) {
    // assert(!(target->position == actor->position));
    if (!(target->position == actor->position)) {
        float distance_module = Position::distance(target->position, actor->position);  // d
        float dx = (target->position.x - actor->position.x);                            // d.x
        float dy = (target->position.y - actor->position.y);                            // d.y

        float acc_module = G * actor->mass / pow(distance_module, 2.0);  // a
        float acc_x = acc_module * (dx / distance_module);               // a.x
        float acc_y = acc_module * (dy / distance_module);               // a.y

        target->acceleration.x += acc_x;
        target->acceleration.y += acc_y;
    }
}

/**
 * given a particle t of our particle vector, compute the interaction with the other particles by recursively traversing
 * the qt
 */
void updateParticleRecursive(Node *n, Particle *p, double theta) {
    if (n->children.empty() && !n->particle) {  //  empty node without children
        return;
    } else if (n->particle) {  //  leaf node therefore compute interaction
        computeInteraction(p, n->particle);
        // p->mass = n->particle->mass * 2; // fix with formula
    } else if ((n->length / Position::distance(n->centerOfMass, p->position)) < theta) {
        //  grouping node which can be used for approximating the underlying tree (its subtree 8D)
        // maybe add !n->particle to condition
        Particle actor = Particle(n->totalMass, n->centerOfMass);  // MEMORY LEAK NEW !!!
        computeInteraction(p, &actor);
    } else {
        // grouping node which cant be used for approximating: therefore whe recursively check its
        // children
        for (auto &child : n->children) {
            updateParticleRecursive(child, p, theta);
        }
    }
}

// return an updated version of the particle `p`. does not influence the reference to `p` inside the
// qt
void updateParticle(Quadtree *qt, Particle *p, double theta) { updateParticleRecursive(qt->root, p, theta); }

/**
 * Computes the interactions between particles in the quadtree and moves them. Always acts on the stack declared vector
 * 'particles'
 */
void updateParticles(Quadtree *qt, double theta, double delta) {
    for (auto &p : particles) {
        p.acceleration = Acceleration(0, 0);
    }
    for (auto &p : particles) {
        updateParticle(qt, &p, theta);
    }
    for (auto &p : particles) {
        p.computeDisplacement(delta);
    }

    cout << "Check particles:" << endl;
    for (auto &p : particles) {
        cout << "\tparticle " << p << " at " << &p << endl;
    }
    assert(!particles.empty());
}

int main() {
    int length = 100;
    int count = 25;
    double DELTA = 1, THETA = 1;
    initializeRandomParticles(count, length);
    // initializeSequentialParticles(length);

    Quadtree *qt = new Quadtree(length, particles);
    qt->computeApproximationValues();
    updateParticles(qt, THETA, DELTA);

    qt->rebuild(particles);
    qt->computeApproximationValues();
    updateParticles(qt, THETA, DELTA);

    qt->print();
    qt->printNodes();

    delete qt;
    clearParticles();
    // assert(!qt);
    assert(particles.empty());
}
