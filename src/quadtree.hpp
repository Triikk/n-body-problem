#pragma once

#include "node.hpp"

#include <optional>

class Quadtree {
   public:
    long double length;
    std::optional<Node> root;

   public:
    Quadtree(long double length);
};