#include <iostream>
#include <set>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <thread>
#include <chrono>

#include "quadtree.hpp"
#include "view.hpp"
#include "physics.hpp"

using namespace std;

bool isPositionAvailable(vector<Particle>& particles, Particle& p) {
    for (auto particle : particles) {
        if (Particle::doCollide(particle, p)) return false;
    }
    return true;
}

bool collides(vector<Particle>& particles, Particle& p1) {
    for (auto& p2 : particles) {
        // if (p1 == p2) continue;
        if (Particle::doCollide(p1, p2)) return true;
    }
    return false;
}

// generate randomly particles in all different positions
vector<Particle> generateRandomParticles(int count, int length, double max_mass) {
    vector<Particle> particles;
    // Particle bigParticle = Particle(1e12, Position(50, 50));
    // particles.push_back(bigParticle);
    for (int i = 0; i < count; i++) {
        Particle p;
        do {
            // p = Particle((rand() % 10 + 1) * 1e7,
            //              Position(((float)rand() / RAND_MAX) * length, ((float)rand() / RAND_MAX) * length));
            float mass = (1 - pow(((float)rand() / RAND_MAX), 2.0)) * max_mass;
            p = Particle(mass, Position(((float)rand() / RAND_MAX) * length, ((float)rand() / RAND_MAX) * length));
        } while (collides(particles, p));
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

vector<Particle> initializeBlackHoleSystem(int count) {
    vector<Particle> particles;

    // Define offsets to ensure all positions are positive and centered
    double offsetX = 5e9;  // Offset to ensure all positions are positive in the x-axis
    double offsetY = 5e9;  // Offset to center the positions in the y-axis

    double blackHoleMass = 1e32;
    double blackHoleRadius = 5e8;
    Particle blackHole =
        Particle(blackHoleMass, Position(offsetX + 0.0, offsetY + 0.0), Velocity(0.0, 0.0), blackHoleRadius);
    particles.push_back(blackHole);

    for (int i = 1; i < count; i++) {
        Particle p;
        do {
            float mass = 1e2;
            double distance = ((double)rand() / RAND_MAX) * offsetX;
            double angle = (rand() % 360) * M_PI / 180;
            double x = offsetX + distance * cos(angle);
            double y = offsetY + distance * sin(angle);
            Position pos = Position(x, y);
            double velocityMagnitude = sqrt(G * blackHoleMass / distance);
            Velocity vel = Velocity(-velocityMagnitude * sin(angle), velocityMagnitude * cos(angle));
            p = Particle(mass, pos, vel, 1e7);
        } while (collides(particles, p));
        particles.push_back(p);
    }
    return particles;
}

int main() {
    // int length = 100;
    // int count = 5;
    double delta = 0.5, theta = 1.0;
    // float mass = 1e11;
    // vector<Particle> particles = generateRandomParticles(count, length, mass);

    float length = 1e10;
    int count = 1000;
    vector<Particle> particles = initializeBlackHoleSystem(count);

    Quadtree qt = Quadtree(length, particles);
    View view = View(qt, 800);

    int i = 0;
    int numIterations = 100;
    auto start = std::chrono::high_resolution_clock::now();

    while (!qt.particles.empty()) {
        cout << "iteration " << i++ << "\n";
        qt.build();
        // cout << "after build: " << qt << endl;
        qt.computeApproximationValues();
        // cout << "after approximationValues: " << qt << endl;
        qt.updateParticles(theta, delta);
        assert(!qt.particles.empty());
        // cout << "after update: " << qt << endl;

        if (i % 20 == 0) {
            view.clear();  // clear the buffer
            view.loadParticles(qt.particles);
            // view.loadTree(qt);
            view.render();
        }

        if (i % numIterations == 0) {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            cout << "Time taken for iterations: " << elapsed.count() / numIterations << " seconds" << endl;
            start = std::chrono::high_resolution_clock::now();  // Reset the start time
        }

        qt.clean();

        // std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}
