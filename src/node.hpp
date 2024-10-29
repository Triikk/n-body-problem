#pragma once

#include "node.hpp"
#include "particle.hpp"

#include <optional>
#include <vector>

class Node {
   public:
    Node* father;
    std::vector<Node*> children;
    Position origin;
    long double length;
    std::optional<Particle> particle;

   public:
    Node(Position origin, long double length);
};