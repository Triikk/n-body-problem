#include "node.hpp"

#include <iostream>
#include <cassert>

Node::Node(Position origin, long double length, Particle *particle)
    : origin{origin}, length{length}, particle{particle}, totalMass{0} {}

void Node::split() {
    assert(children.empty());
    // children = vector<Node *>();
    children.push_back(new Node(Position(origin.x, origin.y), length / 2, nullptr));
    children.push_back(new Node(Position(origin.x, origin.y + length / 2), length / 2, nullptr));
    children.push_back(new Node(Position(origin.x + length / 2, origin.y), length / 2, nullptr));
    children.push_back(new Node(Position(origin.x + length / 2, origin.y + length / 2), length / 2, nullptr));
}

Node::~Node() {
    if (!children.empty()) {
        delete children[0];
        delete children[1];
        delete children[2];
        delete children[3];
    }
    children.clear();
    // si cancella da solo
}

ostream &operator<<(ostream &os, const Node &n) {
    os << "Node<origin=" << n.origin << ",length=" << n.length << ",particle=" << *n.particle << ">";
    return os;
}