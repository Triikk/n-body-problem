#pragma once

#include <vector>

#include "particle.hpp"

using namespace std;

class View {
   public:
    View(int width, int height, vector<Particle> particles);
    ~View();
    void update();
};