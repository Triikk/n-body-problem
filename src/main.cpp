#include <iostream>
#include <set>
#include <cassert>

#include "quadtree.hpp"

#define G 6.67430e-11

using namespace std;

set<Particle *> particles;

bool isOutside(Quadtree *qt, Particle *p) {
    return p->position.x < 0 || p->position.x > qt->length || p->position.y < 0 ||
           p->position.y > qt->length;  // 0 is qt origin
}

void clearParticles() {
    for (auto it = particles.begin(); it != particles.end(); it++) {
        Particle *p = *it;
        delete p;
    }
    particles.clear();
}

// generate randomly particles in all different positions
void initializeRandomParticles(int count, int length) {
    for (int i = 0; i < count; i++) {
        Particle *p;
        do {
            p = new Particle(1, Position(rand() % length, rand() % length));
        } while (particles.find(p) != particles.end());
        cout << "\t" << *p << " at " << p << ")" << endl;
        particles.insert(p);
    }
}

// generate particles sequentially
void initializeSequentialParticles(double max) {
    for (int x = 0; x < max; x++) {
        for (int y = 0; y < max; y++) {
            Particle *p = new Particle(1, Position(x, y));
            cout << "\t" << *p << " at " << p << ")" << endl;
            particles.insert(p);
        }
    }
}

// compute the force between the two particles. G*m1*m2/r^2. Apply the force to target, moving it by
// a certain displacement. if the particles are in the same position the interaction is irrelevant
void computeInteraction(Particle *target, Particle *actor, double delta) {
    assert(!(target->position == actor->position));
    float distance_module = Position::distance(target->position, actor->position);  // d
    float force_module = G * (target->mass * actor->mass) / (distance_module);      // f
    std::vector<float> distance_components;
    distance_components.push_back(target->position.x - actor->position.x);  // dx
    distance_components.push_back(target->position.y - actor->position.y);  // dy
    std::vector<float> force_components;
    force_components.push_back((force_module / distance_module) * distance_components[0]);  // fx
    force_components.push_back((force_module / distance_module) * distance_components[1]);  // fy
    float dispX = force_components[0] / target->mass;                                       // dispX
    float dispY = force_components[1] / target->mass;                                       // dispY
    target->position.x += dispX * delta;
    target->position.y += dispY * delta;
}

// given a particle t of our particle set, compute the interaction with the set particles by recursively traversing the
// qt
void updateParticleRecursive(Node *n, Particle *p, double theta, double delta) {
    if (n->children.empty() && !n->particle) {  //  empty node without children
        return;
    } else if (n->particle) {  //  leaf node therefore compute interaction
        computeInteraction(p, n->particle, delta);
        // p->mass = n->particle->mass * 2; // fix with formula
    } else if ((n->length / Position::distance(n->centerOfMass, p->position)) < theta) {
        //  grouping node which can be used for approximating the underlying tree (its subtree 8D)
        // maybe add !n->particle to condition
        Particle *actor = new Particle(n->totalMass, n->centerOfMass);  // MEMORY LEAK NEW !!!
        computeInteraction(p, actor, delta);
        delete actor;
    } else {
        // grouping node which cant be used for approximating: therefore whe recursively check its
        // children
        for (auto &child : n->children) {
            updateParticleRecursive(child, p, theta, delta);
        }
    }
}

// return an updated version of the particle `p`. does not influence the reference to `p` inside the
// qt
Particle *updateParticle(Quadtree *qt, Particle *p, double theta, double delta) {
    Particle *toUpdate = new Particle(p->mass, p->position);
    updateParticleRecursive(qt->root, toUpdate, theta, delta);
    return toUpdate;
}

// switches the old particle set with a new one where every prtcle of the old one was individually
// updated
void updateParticles(Quadtree *qt, double theta, double delta) {
    set<Particle *> newParticles;
    Particle *tempP;
    for (auto &p : particles) {                       // foreach of the old set we update and insert it in the new one
        tempP = updateParticle(qt, p, theta, delta);  // copia temporanea
        newParticles.insert(tempP);
    }
    particles.clear();
    particles = newParticles;
    assert(!particles.empty());
}

int main() {
    int length = 100;
    int count = 20;
    double delta = 1;
    initializeRandomParticles(count, length);

    Quadtree *qt = new Quadtree(length, particles);
    qt->computeApproximationValues();

    for (auto &p : particles) {
        p->position.x += 0.001;
        p->position.y += 0.001;
    }

    qt->rebuild(particles);
    qt->computeApproximationValues();
    qt->print();
    qt->printNodes();

    delete qt;
    clearParticles();
    // assert(!qt);
    assert(particles.empty());
}
