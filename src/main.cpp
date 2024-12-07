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

#define VIEW

using namespace std;

bool isPositionAvailable(vector<Particle>& particles, Particle& p) {
    bool isAvailable = true;
#pragma omp parallel for
    for (auto particle : particles) {
        if (Particle::doCollide(particle, p)) isAvailable = false;
    }
    return isAvailable;
}

bool collides(vector<Particle>& particles, Particle& p1) {
    bool foundCollisions = false;
#pragma omp parallel for
    for (auto& p2 : particles) {
        if (Particle::doCollide(p1, p2)) foundCollisions = true;
    }
    return foundCollisions;
}

// generate randomly particles in all different positions
vector<Particle> generateRandomParticles(int count, int length, double max_mass, double radius) {
    vector<Particle> particles;
    for (int i = 0; i < count; i++) {
        Particle p;
        do {
            float rand_mass = (float)rand() / RAND_MAX;
            float mass = (1 - rand_mass * rand_mass) * max_mass;
            p = Particle(mass, Position(((float)rand() / RAND_MAX) * length, ((float)rand() / RAND_MAX) * length),
                         Velocity(0.0, 0.0), radius);
        } while (collides(particles, p));
        particles.push_back(p);
    }
    return particles;
}

vector<Particle> generateSequential(double length, int count, float mass, float radius) {
    vector<Particle> particles;
    double num_rows = sqrt(count);
    double row_spacing = length / num_rows;
    for (int i = 0; i < count; i++) {
        double x = (i / (int)num_rows) * row_spacing;
        double y = (i % (int)num_rows) * row_spacing;
        Particle p = Particle(mass, Position(x, y), Velocity(0.0, 0.0), radius);
        particles.push_back(p);
    }
    return particles;
}

vector<Particle> initializeBlackHoleSystem(double length, int count) {
    vector<Particle> particles;

    // Define offsets to ensure all positions are positive and centered
    double offsetX = length / 2;  // Offset to ensure all positions are positive in the x-axis
    double offsetY = length / 2;  // Offset to center the positions in the y-axis

    double blackHoleMass = 1e32;
    double blackHoleRadius = length / 8;
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
            double velocityMagnitude = sqrt(G * blackHoleMass / distance) * (0.95 + (double)rand() / 10 / RAND_MAX);
            Velocity vel = Velocity(-velocityMagnitude * sin(angle), velocityMagnitude * cos(angle));
            p = Particle(mass, pos, vel, blackHoleRadius / 1e2);
        } while (collides(particles, p));
        particles.push_back(p);
    }
    return particles;
}

int main(int argc, char* argv[]) {
    double delta = 0.01, theta = 1, length = 1e10;
    int count = 1e2;
    switch (argc) {
        case 5:
            count = atoi(argv[4]);
        case 4:
            length = atof(argv[3]);
        case 3:
            theta = atof(argv[2]);
            delta = atof(argv[1]);
    }

    vector<Particle> particles = initializeBlackHoleSystem(length, count);
    Quadtree qt = Quadtree(length, particles);

    string windowTitle =
        "N-body simulation (n=" + to_string(count) + ",Δ=" + to_string(delta) + ",θ=" + to_string(theta) + ")";

#ifdef VIEW
    View view = View(qt, 800, windowTitle);
    int renderIterations = 1;
#endif

    int i = 0;
    int numIterations = 100;
    auto start = std::chrono::high_resolution_clock::now();

    while (!qt.particles.empty()) {
        i++;
        qt.build();
        qt.computeApproximationValues();
        qt.updateParticles(theta, delta);
        assert(!qt.particles.empty());

#ifdef VIEW
        if (i % renderIterations == 0) {
            view.clear();  // clear the buffer
            view.loadParticles(qt.particles);
            // view.loadTree(qt);
            view.render();
        }
#endif

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
