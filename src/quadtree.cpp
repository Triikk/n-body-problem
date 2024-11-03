#include "quadtree.hpp"
#include "node.hpp"
#include "position.hpp"

#include <iostream>
#include <set>
#include <cassert>

Quadtree::Quadtree(long double length) : length{length}, root{nullptr} {}

Quadtree::Quadtree(long double length, set<Particle*>& particles) : length{length}, root{nullptr} { build(particles); }

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

void push_down(Node* node) {
    node->split();
    int index = compute_child_index(node, node->particle);
    node->children[index]->particle = node->particle;  // move particle to correct child node
    node->particle = nullptr;                          // remove particle from previous node
}

/**
 * Return the node to insert the particle.
 */
Node* getNodeToInsert(Node* node, Particle* particle) {
    if (node->children.empty() && !node->particle) {  // leaf with no particle
        return node;
    } else if (node->children.empty() && node->particle) {  // leaf with particle
        push_down(node);
        return getNodeToInsert(node, particle);
    } else if (!node->children.empty()) {
        int index = compute_child_index(node, particle);
        std::cout << "getNodeToInsert node: " << *node << ",particle:  " << *particle << std::endl;
        std::cout << "index: " << index << std::endl;
        return getNodeToInsert(node->children[index], particle);
    }
    return nullptr;
}

void Quadtree::add(Particle* particle) {
    // there is no root
    if (!root) {
        root = new Node(Position(0, 0), length, particle);
        return;
    }
    Node* node = getNodeToInsert(root, particle);
    node->particle = particle;
}

/**
 * Update total mass and center of mass of each node below `node`.
 */
void update(Node* node) {
    if (node->children.empty() && !node->particle) {  // leaf with no particle
        node->totalMass = 0;
        return;
    } else if (node->children.empty() && node->particle) {  // leaf with particle
        node->totalMass = abs(node->particle->mass);
        node->centerOfMass = node->particle->position;
        return;
    } else if (!node->children.empty()) {
        for (auto& child : node->children) {
            update(child);
            node->totalMass += child->totalMass;
            node->centerOfMass += child->centerOfMass * child->totalMass;
        }
        node->centerOfMass /= node->totalMass;
        return;
    }
    assert(false);
}

void Quadtree::computeApproximationValues() { update(root); }

void Quadtree::print() { std::cout << "Quadtree<length=" << length << ",root=" << *root << ">" << std::endl; }

void print_nodes(Node* node) {
    if (!node) {
        cout << "\tnode=<null>" << endl;
        return;
    }
    cout << "\t" << *node << endl;
    for (Node* child : node->children) {
        print_nodes(child);
    }
}

void Quadtree::printNodes() { print_nodes(root); }

void Quadtree::clean() {
    cout << "clean tree" << endl;
    delete root;
    root = nullptr;
}

void Quadtree::build(set<Particle*>& particles) {
    cout << "build tree" << endl;
    for (auto& p : particles) {
        cout << "\tadding particle " << *p << " to tree" << endl;
        add(p);
    }
}

void Quadtree::rebuild(set<Particle*>& particles) {
    cout << "rebuild tree" << endl;
    clean();
    assert(!root);
    build(particles);
    assert(root);
}

Quadtree::~Quadtree() { clean(); }