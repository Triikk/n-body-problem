#include "quadtree.hpp"
#include "node.hpp"
#include "position.hpp"

#include <iostream>
#include <set>
#include <cassert>
#include <cmath>
#include <algorithm>

/**
 * Initialization with length and particles
 */
Quadtree::Quadtree(float length, vector<Particle>& particles) : length{length}, root{nullptr}, particles{particles} {}

/**
 * Given a node and a particle, computes the child in which the particle should be placed
 */
int computeChildIndex(Node* node, Particle& particle) {
    // 1 3
    // 0 2
    int gx = particle.position.x > (node->origin.x + node->length / 2);
    int gy = particle.position.y > (node->origin.y + node->length / 2);
    return gx * 2 + gy * 1;
}

void pushDown(Node* node) {
    node->split();
    int index = computeChildIndex(node, *node->particle);
    node->children[index]->particle = node->particle;  // move particle to correct child node
    node->particle = nullptr;                          // remove particle from previous node
}

/**
 * Return the node to insert the particle.
 */
Node* getNodeToInsert(Node* node, Particle& particle) {
    // if ((std::isnan(particle->position.x)) || (std::isnan(particle->position.y))) {
    //     cerr << "LA COLPEVOLE: ABITA IN VIA " << particle << " , A BREVE IL PROCESSO." << endl;
    //     assert(false);
    // }
    if (node->children.empty() && !node->particle) {  // leaf with no particle
        return node;
    } else if (node->children.empty() && node->particle) {  // leaf with particle
        pushDown(node);
        return getNodeToInsert(node, particle);
    } else if (!node->children.empty()) {
        int index = computeChildIndex(node, particle);
        // std::cerr << "getNodeToInsert node: " << *node << ",particle:  " << *particle << std::endl;
        // std::cerr << "index: " << index << std::endl;
        return getNodeToInsert(node->children[index], particle);
    }
    assert(false);
    // return nullptr;
}

void Quadtree::add(Particle& particle) {
    // there is no root
    if (!root) {
        root = new Node(Position(0, 0), length, &particle);
        return;
    }
    // cerr << "particle " << *particle << " at " << particle << endl;
    Node* node = getNodeToInsert(root, particle);
    node->particle = &particle;
}

bool Quadtree::isOutside(Particle& p) {
    return p.position.x < 0.0 || p.position.x > length || p.position.y < 0.0 || p.position.y > length;
}

void Quadtree::recursiveNetForce(Node* n, Particle& p, double theta) {
    if (n->children.empty() && !n->particle) {  //  empty node without children
        return;
    } else if (n->particle) {  //  leaf node therefore compute interaction
        p.computeSingleForce(*n->particle);
    } else if ((n->length / Position::distance(n->centerOfMass, p.position)) < theta) {
        //  grouping node which can be used for approximating the underlying tree (its subtree 8D)
        Particle actor = Particle(n->totalMass, n->centerOfMass);
        p.computeSingleForce(actor);
    } else {
        // grouping node which can't be used for approximation: therefore we recursively check its children
        for (auto& child : n->children) {
            recursiveNetForce(child, p, theta);
        }
    }
}

void Quadtree::computeNetForce(Particle& p, double theta) {
    // cerr << "computeNetForce " << p << endl;
    if (isOutside(p)) {
        return;
    }
    recursiveNetForce(root, p, theta);
}

// traverse the tree recursively and return the vector of particles which are centered inside the area of collision
// of p
void findCollidingParticles(Particle& p, Node* node, vector<Particle*>& colliding_particles) {
    if (node->children.empty() && !node->particle) {  //  empty node without children
        return;
    } else if (node->particle) {  //  leaf node therefore check if the particle collides
        // TO DO
        if (*node->particle == p) return;
        if (Particle::doCollide(p, *node->particle)) {
            colliding_particles.push_back(node->particle);
        }
        return;
    } else {
        // father node: recursively call on plausible children
        for (auto& child : node->children) {
            if (node->containsCollisionArea(p)) findCollidingParticles(p, child, colliding_particles);
        }
    }
}

void Quadtree::manageCollisions(double delta) {
    bool foundCollisions = false;
    // do {
    for (auto& p : particles) {
        p.colliding_particles.clear();
        p.acceleration = Acceleration(0, 0);
    }
    foundCollisions = false;
    for (auto& p : particles) {
        vector<Particle*> colliding_particles;
        findCollidingParticles(p, root, colliding_particles);
        if (!(colliding_particles.empty())) {
            for (auto& p1 : colliding_particles) {
                if (find(p1->colliding_particles.begin(), p1->colliding_particles.end(), &p) ==
                    p1->colliding_particles.end()) {
                    p.addCollidingParticle(p1);
                    p1->addCollidingParticle(&p);
                }
            }
            foundCollisions = true;
        }
        // delete colliding_particles;
    }
    if (foundCollisions) {
        for (auto& p : particles) {
            p.computeCollisions();
        }
        for (auto& p : particles) {
            p.computeCollisionDisplacement(delta);
        }
    }
    // } while (foundCollisions);
}

/**
 * Update the state of each particle after one round of interaction.
 * @param qt quadtree
 * @param theta threshold for the approximation
 * @param delta time step
 * @param particles vector of particles
 */
void Quadtree::updateParticles(double theta, double delta) {
    for (auto& p : particles) {
        p.acceleration = Acceleration(0, 0);
    }
    for (auto& p : particles) {
        computeNetForce(p, theta);
    }
    // remove particles outside the quadtree
    particles.erase(std::remove_if(particles.begin(), particles.end(), [&](Particle& p) { return isOutside(p); }),
                    particles.end());
    for (auto& p : particles) {
        p.computeDisplacement(delta);
    }
    manageCollisions(delta);
    assert(!particles.empty());
}

/**
 * Update total mass and center of mass of each node below `node`.
 */
void update(Node* node) {
    if (!node) return;
    if (node->children.empty() && !node->particle) {  // leaf with no particle
        node->totalMass = 0;
        // node->centerOfMass = Position(0, 0);
        return;
    } else if (node->children.empty() && node->particle) {  // leaf with particle
        node->totalMass = node->particle->mass;
        node->centerOfMass = node->particle->position;
        return;
    } else if (!node->children.empty()) {
        node->totalMass = 0;
        node->centerOfMass = Position(0, 0);
        // node->centerOfMass = Position(node->origin.x + node->length / 2, node->origin.y + node->length / 2);
        for (auto& child : node->children) {
            update(child);
            node->totalMass += child->totalMass;
            node->centerOfMass += child->centerOfMass * child->totalMass;
        }
        // cout << "com " << node->centerOfMass << endl;
        node->centerOfMass /= node->totalMass;
        return;
    }
    assert(false);
}

/**
 * Compute centers of mass
 */
void Quadtree::computeApproximationValues() { update(root); }

void printNodesRecursive(ostream& os, Node* node) {
    if (!node) {
        os << "\tnode=<null>" << endl;
        return;
    }
    os << "\t" << *node << endl;
    for (Node* child : node->children) {
        printNodesRecursive(os, child);
    }
}

ostream& operator<<(ostream& os, Quadtree& qt) {
    if (!qt.root) {
        return os;
    }
    os << "Quadtree<length=" << qt.length << ",root=" << *qt.root << ">" << endl;
    printNodesRecursive(os, qt.root);
    return os;
}

void Quadtree::clean() {
    // cerr << "clean tree" << endl;
    delete root;
    root = nullptr;
}

/**
 * adds every particle to the correct quadrant of the tree
 */
void Quadtree::build() {
    // cerr << "build tree" << endl;
    for (auto& p : particles) {
        if (isOutside(p)) {
            continue;
        }
        // cerr << "\tadding particle " << p << " at " << &p << " to tree" << endl;
        add(p);
    }
    // computeApproximationValues();
}

// void Quadtree::rebuild(vector<Particle>& particles) {
//     cerr << "rebuild tree" << endl;
//     clean();
//     assert(!root);
//     build(particles);
//     assert(root);
// }

Quadtree::~Quadtree() { clean(); }