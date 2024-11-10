#pragma once

#include <vector>

#include "particle.hpp"
#include "quadtree.hpp"

using namespace std;

class View {
   public:
    Quadtree& qt;

   public:
    View(Quadtree& qt, int windowLength);
    ~View();
    void update();
    void drawTree();
};