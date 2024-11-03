#include <iostream>
#include <set>
#include <cassert>

#include "quadtree.hpp"

using namespace std;

void print_nodes(Node* node) {
    if (!node) {
        cout << "\tnode is nullptr" << endl;
        return;
    }
    // cout << "sono in dfs con nodo " << *node << endl;
    if (node->particle) {
        // print origin, particle position, center of mass, total mass with labels
        cout << "\t" << *node << endl;
    }
    for (Node* child : node->children) {
        print_nodes(child);
    }
}

void print_tree(Quadtree* qt) {
    cout << "print_tree" << endl;
    print_nodes(qt->root);
}

set<Particle*> particles;

bool is_outside(Quadtree* qt, Particle* p) {
    return p->position.x < 0 || p->position.x > qt->length || p->position.y < 0 ||
           p->position.y > qt->length;  // 0 is qt origin
}

Quadtree* init(double length) {
    cout << "init" << endl;
    Quadtree* qt = new Quadtree(length);
    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            Particle* p = new Particle(1, Position(x, y));
            cout << "\t" << *p << " at " << p << ")" << endl;
            particles.insert(p);
            qt->add(p);
        }
    }
    return qt;
}

void clean_tree(Quadtree* qt) {
    cout << "\tclean_tree" << endl;
    delete qt->root;
    qt->root = nullptr;
}

void build_tree(Quadtree* qt) {
    cout << "build_tree" << endl;
    // safely modifying iterable while iterating
    for (auto it = particles.begin(); it != particles.end(); it++) {
        Particle* p = *it;
        if (is_outside(qt, p)) {
            cout << "\terasing from set particle " << *p << endl;
            delete p;
            particles.erase(p);
        } else {
            cout << "\tadding particle " << *p << " to tree" << endl;
            qt->add(p);
        }
    }
}

void rebuild_tree(Quadtree* qt) {
    cout << "rebuild_tree" << endl;
    clean_tree(qt);
    assert(!qt->root);
    build_tree(qt);
    assert(qt->root);
}

void destroy_tree(Quadtree*& qt) {
    cout << "destroy_tree" << endl;
    clean_tree(qt);
    delete qt;
    qt = nullptr;
}

void clear_particles() {
    for (auto it = particles.begin(); it != particles.end(); it++) {
        Particle* p = *it;
        delete p;
    }
    particles.clear();
}

int main() {
    Quadtree* qt = init(10);
    qt->computeApproximationValues();

    for (auto& p : particles) {
        p->position.x += 0.001;
        p->position.y += 0.001;
    }

    clean_tree(qt);
    assert(!qt->root);  // qt root is nullptr

    build_tree(qt);
    assert(qt->root);

    rebuild_tree(qt);

    // clean_tree(qt);
    // assert(!qt->root);
    print_tree(qt);

    destroy_tree(qt);
    clear_particles();
    assert(!qt);
    assert(particles.empty());

    // for (auto it = particles.begin(); it != particles.end(); it++) {
    //     Particle* p = *it;
    //     cout << "erasing " << p << "(" << *p << ")" << endl;
    //     delete p;
    // }
    // particles.clear();
    // dfs(qt->root);

    // delete qt;
}
