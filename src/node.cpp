#include "node.hpp"

Node::Node(Position origin, long double length, Particle* particle)
    : origin{origin}, length{length}, particle{particle} {}

void Node::split() {
    children = std::vector<Node*>(4);
    children[0] = new Node(Position(origin.x, origin.y), length / 2, nullptr);
    children[1] = new Node(Position(origin.x, origin.y + length / 2), length / 2, nullptr);
    children[2] = new Node(Position(origin.x + length / 2, origin.y), length / 2, nullptr);
    children[3] = new Node(Position(origin.x + length / 2, origin.y + length / 2), length / 2, nullptr);
}