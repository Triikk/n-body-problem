#include <iostream>
#include <set>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <thread>
#include <chrono>

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
    // Particle bigParticle = Particle(1e12, Position(50, 50));
    // particles.push_back(bigParticle);
    for (int i = 0; i < count; i++) {
        Particle p;
        do {
            // p = Particle((rand() % 10 + 1) * 1e7,
            //              Position(((float)rand() / RAND_MAX) * length, ((float)rand() / RAND_MAX) * length));
            p = Particle(mass, Position(((float)rand() / RAND_MAX) * length, ((float)rand() / RAND_MAX) * length));
        } while (!isPositionAvailable(particles, p.position));
        // // double dx = p.position.x - 50;
        // // double dy = p.position.y - 50;
        // // double theta = acos(dx / Position::distance(p.position, bigParticle.position));
        // // p.velocity.x = dx * sin(theta);
        // // p.velocity.y = dy * cos(theta);
        // // double norm = sqrt(pow(p.velocity.x, 2) + pow(p.velocity.y, 2));  // velocity unit vector
        // // p.velocity /= (norm / (rand() % 10));
        // p.velocity.y = (rand() % 10) * 1;
        particles.push_back(p);
        // cout << p.radius << endl;
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
    int count = 20;
    double delta = 0.01, theta = 1;
    float mass = 1e10;
    vector<Particle> particles = generateRandomParticles(count, length, mass);

    Quadtree qt = Quadtree(length, particles);
    View view = View(qt, 800);

    while (!qt.particles.empty()) {
        qt.build();
        // cout << "after build: " << qt << endl;
        qt.computeApproximationValues();
        // cout << "after approximationValues: " << qt << endl;
        qt.updateParticles(theta, delta);
        // cout << "after update: " << qt << endl;

        view.clear();  // clear the buffer
        view.loadParticles(qt.particles);
        // view.loadTree(qt);
        view.render();

        qt.clean();

        // std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}
