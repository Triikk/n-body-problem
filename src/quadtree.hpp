#pragma once

#include "node.hpp"

#include <optional>

class Quadtree {
   public:
    long double length;
    Node* root;

   public:
    Quadtree(long double length);
    void add(Particle* particle);
};