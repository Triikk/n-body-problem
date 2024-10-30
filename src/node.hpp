#pragma once

#include "node.hpp"
#include "particle.hpp"

#include <optional>
#include <vector>

class Node {
   public:
    std::vector<Node*> children;
    Position origin;
    long double length;
    Particle* particle;
    Position centerOfMass;
    double totalMass;

    Node(Position origin, long double length, Particle* particle);
    void split();
};