#include "node.hpp"

#include <iostream>
#include <cassert>

Node::Node(Position origin, long double length, Particle *particle)
    : origin{origin}, length{length}, particle{particle}, totalMass{0} {}

void Node::split() {
    assert(children.empty());
    children.push_back(new Node(Position(origin.x, origin.y), length / 2, nullptr));
    children.push_back(new Node(Position(origin.x, origin.y + length / 2), length / 2, nullptr));
    children.push_back(new Node(Position(origin.x + length / 2, origin.y), length / 2, nullptr));
    children.push_back(new Node(Position(origin.x + length / 2, origin.y + length / 2), length / 2, nullptr));
}

bool Node::containsCollisionArea(Particle &p) {
    Position pBottom = Position(p.position.x - 2 * p.radius, p.position.y - 2 * p.radius);
    Position pTop = Position(p.position.x + 2 * p.radius, p.position.y + 2 * p.radius);
    Position nBottom = Position(origin.x, origin.y);
    Position nTop = Position(origin.x + length, origin.y + length);
    return (pBottom <= nTop) && (nBottom <= pTop);
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
    os << "N<org=" << n.origin << ",length=" << n.length << ",tot.mass=" << n.totalMass << ",com=" << n.centerOfMass;
    if (n.particle) {
        os << *n.particle;
    } else {
        os << ",Particle=" << "none";
    }
    os << ">";
    return os;
}