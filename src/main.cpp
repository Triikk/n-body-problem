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
    // Particle bigParticle = Particle(1e12, Position(50, 50));
    // particles.push_back(bigParticle);
    for (int i = 0; i < count; i++) {
        Particle p;
        do {
            // p = Particle((rand() % 10 + 1) * 1e7,
            //              Position(((float)rand() / RAND_MAX) * length, ((float)rand() / RAND_MAX) * length));
            float rand_mass = (float)rand() / RAND_MAX;
            float mass = (1 - rand_mass * rand_mass) * max_mass;
            p = Particle(mass, Position(((float)rand() / RAND_MAX) * length, ((float)rand() / RAND_MAX) * length),
                         Velocity(0.0, 0.0), radius);
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
            double velocityMagnitude = sqrt(G * blackHoleMass / distance);
            Velocity vel = Velocity(-velocityMagnitude * sin(angle), velocityMagnitude * cos(angle));
            p = Particle(mass, pos, vel, 1e7);
        } while (collides(particles, p));
        // } while (false);
        particles.push_back(p);
    }
    return particles;
}

int main(int argc, char* argv[]) {
    double delta = 0.5, theta = 1;
    if (argc > 2) {
        delta = atof(argv[0]);
        theta = atof(argv[1]);
    }

    // int length = 100;
    // int count = 5;
    // float mass = 1e11;
    // vector<Particle> particles = generateRandomParticles(count, length, mass);

    double length = 5e10;
    int count = 1e5;
    vector<Particle> particles = initializeBlackHoleSystem(length, count);
    // vector<Particle> particles = generateRandomParticles(count, length, 1e5, 1e2);

    Quadtree qt = Quadtree(length, particles);

#ifdef VIEW
    View view = View(qt, 800);
    int renderIterations = 1;
#endif

    int i = 0;
    int numIterations = 100;
    auto start = std::chrono::high_resolution_clock::now();

    while (!qt.particles.empty()) {
        i++;
        // cout << "iteration " << i << "\n";
        qt.build();
        // cout << "after build: " << qt << endl;
        qt.computeApproximationValues();
        // cout << "after approximationValues: " << qt << endl;
        qt.updateParticles(theta, delta);
        assert(!qt.particles.empty());
        // cout << "after update: " << qt << endl;

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
