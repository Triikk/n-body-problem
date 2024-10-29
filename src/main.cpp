#include <iostream>
#include "quadtree.hpp"

void dfs(Node* node) {
    if (node == nullptr) {
        return;
    }
    if (node->children.empty() && node->particle) {
        std::cout << "Node at " << node->origin.x << " " << node->origin.y << " has particle at "
                  << node->particle->position.x << " " << node->particle->position.y << std::endl;
        return;
    }
    for (Node* child : node->children) {
        dfs(child);
    }
}

int main() {
    Quadtree* qt = new Quadtree(64);
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 64; y++) {
            Particle* p = new Particle(1, Position(x, y));
            qt->add(p);
        }
    }

    dfs(qt->root);
    // dfs to print qt
}
