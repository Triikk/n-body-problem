#include <iostream>
#include "quadtree.hpp"

void dfs(Node* node) {
    if (node == nullptr) {
        return;
    }
    if (node->children.empty() && node->particle) {
        // print origin, particle position, center of mass, total mass with labels
        std::cout << "Origin: " << node->origin.x << "," << node->origin.y
                  << "Particle position: " << node->particle->position.x << "," << node->particle->position.y
                  << "Center of mass: " << node->centerOfMass.x << "," << node->centerOfMass.y
                  << "Total mass: " << node->totalMass << std::endl;

        return;
    }
    for (Node* child : node->children) {
        dfs(child);
    }
}

int main() {
    Quadtree* qt = new Quadtree(16);
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            Particle* p = new Particle(1, Position(x, y));
            qt->add(p);
        }
    }
    qt->computeApproximationValues();

    dfs(qt->root);
    // dfs to print qt
}
