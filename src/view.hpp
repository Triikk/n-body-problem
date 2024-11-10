#pragma once

#include <vector>

#include "particle.hpp"
#include "quadtree.hpp"

using namespace std;

class View {
   public:
    View(Quadtree& qt, int windowLength);
    ~View();
    void clear();
    void render();
    void loadParticles(vector<Particle>& particles);
    void loadTree(Quadtree& qt);
};