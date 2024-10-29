#include "quadtree.hpp"
#include "node.hpp"
#include "position.hpp"

Quadtree::Quadtree(long double length) : length{length} {
    this->root = std::nullopt;
}