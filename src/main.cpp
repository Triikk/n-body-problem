#include <iostream>
#include <set>
#include <cassert>

#include "quadtree.hpp"

using namespace std;

set<Particle*> particles;

bool isOutside(Quadtree* qt, Particle* p) {
    return p->position.x < 0 || p->position.x > qt->length || p->position.y < 0 ||
           p->position.y > qt->length;  // 0 is qt origin
}

void clearParticles() {
    for (auto it = particles.begin(); it != particles.end(); it++) {
        Particle* p = *it;
        delete p;
    }
    particles.clear();
}

void initializeRandomParticles(int count, int length) {
    for (int i = 0; i < count; i++) {
        Particle* p;
        do {
            p = new Particle(1, Position(rand() % length, rand() % length));
        } while (particles.find(p) != particles.end());
        cout << "\t" << *p << " at " << p << ")" << endl;
        particles.insert(p);
    }
}

void initializeSequentialParticles(double max) {
    for (int x = 0; x < max; x++) {
        for (int y = 0; y < max; y++) {
            Particle* p = new Particle(1, Position(x, y));
            cout << "\t" << *p << " at " << p << ")" << endl;
            particles.insert(p);
        }
    }
}

void updateParticleRecursive(Node* n, Particle* p, double theta) {
    if (n->children.empty() && !n->particle) {
        // empty node without children
        return;
    } else if (n->particle) {
        // we reached a leaf so we just calculate the force
        // p->mass = n->particle->mass * 2; // fix with formula
    } else if ((n->length / Position::distance(n->centerOfMass, p->position)) < theta) {
        // maybe add !n->particle to condition
        // p->mass = n->particle->mass * something; // fix with formula
    } else {
        for (auto& child : n->children) {
            updateParticleRecursive(child, p, theta);
        }
    }
}

void updateParticle(Quadtree* qt, Particle* p, double theta) {}

void updateParticles(Quadtree* qt, double theta) {
    set<Particle*> tmpParticles;
    for (auto& p : particles) {
        tmpParticles.insert(new Particle(p->mass, p->position));
    }
    for (auto& p : tmpParticles) {
        updateParticle(qt, p, theta);  // copia temporanea
    }
}

int main() {
    int length = 100;
    int count = 20;
    initializeRandomParticles(count, length);

    Quadtree* qt = new Quadtree(length, particles);
    qt->computeApproximationValues();

    for (auto& p : particles) {
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
