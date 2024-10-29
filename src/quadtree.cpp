#include "quadtree.hpp"
#include "node.hpp"
#include "position.hpp"

Node* dfs(Node* node, Particle* particle);
void push_down(Node* node);
int compute_child_index(Node* node, Particle* particle);

Quadtree::Quadtree(long double length) : length{length}, root{nullptr} {}

void Quadtree::add(Particle* particle) {
    // there is no root
    if (root == nullptr) {
        root = new Node(particle->position, length, particle);
        return;
    }
    Node* node = dfs(root, particle);
    node->particle = particle;
}

Node* dfs(Node* node, Particle* particle) {
    if (node->children.empty() && node->particle) {
        push_down(node);
        return dfs(node, particle);
    } else if (node->children.empty() && !node->particle) {
        return node;
    } else if (!node->children.empty()) {
        int index = compute_child_index(node, particle);
        return dfs(node->children[index], particle);
    }
    return nullptr;
}

void push_down(Node* node) {
    node->split();
    int index = compute_child_index(node, node->particle);
    node->children[index]->particle = node->particle;  // move particle to correct child node
    node->particle = nullptr;                          // remove particle from previous node
}

/**
 * Given a node and a particle, computes the child in which the particle should be placed
 */
int compute_child_index(Node* node, Particle* particle) {
    // 1 3
    // 0 2
    int gx = particle->position.x > (node->origin.x + node->length / 2);
    int gy = particle->position.y > (node->origin.y + node->length / 2);
    return gx * 2 + gy * 1;
}