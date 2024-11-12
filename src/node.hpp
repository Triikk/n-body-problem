#pragma once

#include "node.hpp"
#include "particle.hpp"

#include <optional>
#include <vector>

using namespace std;

class Node {
   public:
    vector<Node*> children;
    Position origin;
    long double length;
    Particle* particle;
    Position centerOfMass;
    double totalMass;

    Node(Position origin, long double length, Particle* particle);
    void split();
    bool containsCollisionArea(Particle& p);
    ~Node();
    friend ostream& operator<<(ostream& os, const Node& n);
};