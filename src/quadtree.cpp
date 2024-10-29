#include "quadtree.hpp"
#include "node.hpp"
#include "position.hpp"

void push_down(Node* node);

Quadtree::Quadtree(long double length) : length{length}, root{nullptr} {}

void Quadtree::add(Particle* particle) {
    // there is no root
    if (root == nullptr) {
        root = new Node(particle->position, length, particle);
        return;
    }
}

Node* dfs(Node* node, Particle* particle) {
    if (node->children.empty() && node->particle) {
        push_down(node);
        return dfs(node, particle);
    } else if (node->children.empty() && !node->particle) {
        node->particle = particle;
    } else {
    }
}

void push_down(Node* node) {
    // 1 3
    // 0 2
    int gx = node->particle->position.x > (node->origin.x + node->length / 2);
    int gy = node->particle->position.y > (node->origin.y + node->length / 2);
    int index = gx * 2 + gy * 1;

    node->split();
    node->children[index]->particle = node->particle;  // move particle to correct child node
    node->particle = nullptr;                          // remove particle from previous node
}