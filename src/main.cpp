#include <iostream>
#include <set>
#include <cassert>
#include <cmath>
#include <algorithm>

#include "quadtree.hpp"
#include "view.hpp"

using namespace std;

bool isPositionAvailable(vector<Particle>& particles, Position p1) {
    for (auto particle : particles) {
        if (p1 == particle.position) return false;
    }
    return true;
}

// generate randomly particles in all different positions
vector<Particle> generateRandomParticles(int count, int length, float mass) {
    vector<Particle> particles;
    for (int i = 0; i < count; i++) {
        Particle p;
        do {
            p = Particle(mass, Position(((float)rand() / RAND_MAX) * length, ((float)rand() / RAND_MAX) * length));
        } while (!isPositionAvailable(particles, p.position));
        particles.push_back(p);
    }
    /*cerr << "InitParticles:" << endl;
    for (auto &p : particles) {
        cerr << "\tparticle " << p << " at " << &p << endl;
    }*/
    return particles;
}

vector<Particle> generateSequential(double max, float mass) {
    vector<Particle> particles;
    for (int x = 0; x < max; x++) {
        for (int y = 0; y < max; y++) {
            Particle p = Particle(mass, Position(x, y));
            // cerr << "\t" << p << " at " << &p << ")" << endl;
            particles.push_back(p);
        }
    }
    return particles;
}

int main() {
    int length = 100;
    int count = 500;
    double delta = 0.001, theta = 1;
    float mass = 1e10;
    vector<Particle> particles = generateRandomParticles(count, length, mass);

    Quadtree qt = Quadtree(length, particles);
    View view = View(qt, 800);

    while (true) {
        qt.build();
        qt.computeApproximationValues();
        qt.updateParticles(theta, delta);

        view.clear();  // clear the buffer
        view.loadParticles(qt.particles);
        // view.loadTree(qt);
        view.render();

        qt.clean();
    }
}
