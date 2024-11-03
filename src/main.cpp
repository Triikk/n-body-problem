#include <iostream>
#include <set>
#include <cassert>

#include "quadtree.hpp"

using namespace std;

set<Particle*> particles;

bool is_outside(Quadtree* qt, Particle* p) {
    return p->position.x < 0 || p->position.x > qt->length || p->position.y < 0 ||
           p->position.y > qt->length;  // 0 is qt origin
}

// Quadtree* init(double length) {
//     cout << "init" << endl;
//     Quadtree* qt = new Quadtree(length);
//     for (int x = 0; x < length; x++) {
//         for (int y = 0; y < length; y++) {
//             Particle* p = new Particle(1, Position(x, y));
//             cout << "\t" << *p << " at " << p << ")" << endl;
//             particles.insert(p);
//             qt->add(p);
//         }
//     }
//     return qt;
// }

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
