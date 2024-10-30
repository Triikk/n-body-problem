#include <iostream>
#include <set>
#include "quadtree.hpp"

void dfs(Node* node) {
    if (node == nullptr) {
        return;
    }
    if (node) {
        // print origin, particle position, center of mass, total mass with labels
        std::cout << "Origin: " << node->origin.toString() << " Center of mass: " << node->centerOfMass.toString()
                  << " Length " << node->length << " Total mass: " << node->totalMass << std::endl;
    }
    for (Node* child : node->children) {
        dfs(child);
    }
}

std::set<Particle*> particles;

bool is_inside(Quadtree* qt, Particle* p) {
    return p->position.x < 0 || p->position.x > qt->length || p->position.y < 0 ||
           p->position.y < qt->length;  // 0 is qt origin
}

Quadtree* init(double length) {
    Quadtree* qt = new Quadtree(length);
    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            Particle* p = new Particle(1, Position(x, y));
            particles.insert(p);
            qt->add(p);
        }
    }
    return qt;
}

// void clean(Node* node) {
//     if (!node) {
//         return;
//     }
//     if (node->children.empty()) {
//         delete node;
//         return;
//     }
//     delete node;
// }

void clean_tree(Quadtree* qt) {
    // clean(qt->root);
    delete qt->root;
    qt->root = nullptr;
}

void build_tree(Quadtree* qt) {
    std::cout << qt->root << std::endl;
    // safely modifying iterable while iterating
    for (auto it = particles.begin(); it != particles.end(); it++) {
        Particle* p = *it;
        if (is_inside(qt, p)) {
            particles.erase(p);
        } else {
            qt->add(p);
        }
    }
}

void rebuild_tree(Quadtree* qt) {
    clean_tree(qt);
    build_tree(qt);
}

int main() {
    Quadtree* qt = init(2);
    std::cout << qt->root << std::endl;
    qt->computeApproximationValues();

    for (auto& p : particles) {
        p->position.x += 0.001;
        p->position.y += 0.001;
    }
    clean_tree(qt);
    build_tree(qt);
    // rebuild_tree(qt);
    // for (auto& p : particles) {
    //     p->position.x += 0.001;
    //     p->position.y += 0.001;
    // }
    // rebuild_tree(qt);
    // for (auto& p : particles) {
    //     p->position.x += 0.001;
    //     p->position.y += 0.001;
    // }
    // rebuild_tree(qt);

    // show_frame();

    // for (int round = 0; round < rounds; round++) {
    //     update_particles();
    //     destroy_tree();
    //     show_frame();
    // rebuild_tree(qt);
    //     build_tree()->computeApproximationValues(); -> computeA
    // }

    Position p;
    std::cout << p.toString() << std::endl;

    dfs(qt->root);
    // dfs to print qt
}
