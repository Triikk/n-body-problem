#pragma once

#include "node.hpp"

#include <optional>
#include <set>

class Quadtree {
   public:
    float length;
    Node* root;
    vector<Particle>& particles;

   public:
    Quadtree(float length, vector<Particle>& particles);
    ~Quadtree();
    void add(Particle& particle);
    void print();
    void printNodes();
    void clean();
    void build();
    void computeApproximationValues();
    void updateParticles(double theta, double delta);
    bool isOutside(Particle& p);

   private:
    void computeNetForce(Particle& p, double theta);
    void recursiveNetForce(Node* node, Particle& p, double theta);
};