#pragma once

#include "node.hpp"

#include <optional>
#include <set>

class Quadtree {
   public:
    long double length;
    Node* root;

   public:
    Quadtree(long double length);
    Quadtree(long double length, vector<Particle>& particles);
    ~Quadtree();
    void add(Particle* particle);
    void computeApproximationValues();
    void print();
    void printNodes();
    void clean();
    void build(vector<Particle>& particles);
    void rebuild(vector<Particle>& particles);
};