#include "node.hpp"

#include <iostream>

Node::Node(Position origin, long double length, Particle* particle)
    : origin{origin}, length{length}, particle{particle}, totalMass{0} {}

void Node::split() {
    children = std::vector<Node*>(4);
    children[0] = new Node(Position(origin.x, origin.y), length / 2, nullptr);
    children[1] = new Node(Position(origin.x, origin.y + length / 2), length / 2, nullptr);
    children[2] = new Node(Position(origin.x + length / 2, origin.y), length / 2, nullptr);
    children[3] = new Node(Position(origin.x + length / 2, origin.y + length / 2), length / 2, nullptr);
}

Node::~Node() {
    // std::cout << "sto chiamando " << length << "," << origin.toString() << " " << particle->toString() << std::endl;
    if (children.empty()) {
        std::cout << "arrivo" << std::endl;
        delete children[0];
        delete children[1];
        delete children[2];
        delete children[3];
    }
    // si cancella da solo
}