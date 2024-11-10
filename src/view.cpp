#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <chrono>
#include <thread>

#include "view.hpp"

using namespace std;
using namespace glm;

GLuint matProj;

const char *vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
uniform mat4 projection;
void main() {
    gl_Position = projection*vec4(aPos, 0.0, 1.0);
}
)";

// layout(location = 1) in mat4 model;
// model*vec4

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
)";

GLuint compileShader(GLenum type, const char *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    return shader;
}

GLuint initializeShader() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

GLuint VBO, VAO, VBOqt, VAOqt;
// void setupBuffers(vector<Particle> particles) {
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);

//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);

//     vector<vec2> positions;
//     for (auto &p : particles) {
//         positions.push_back(vec2(p.position.x, p.position.y));
//     }
//     glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec2), particles.data(), GL_DYNAMIC_DRAW);

//     glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, (void *)0);
//     glEnableVertexAttribArray(0);

//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);
// }

#include <iostream>
#include "quadtree.hpp"
#include <cassert>

#define G 6.674304

GLuint shaderProgram;
GLFWwindow *window;
vector<vec2> positions;
mat4 projection;

vector<Node> getQuadtreeNodes(Node *node) {
    if (!node) {
        cout << "not node" << endl;
        return {};
    } else if (node->children.empty()) {
        return {*node};
    } else {
        vector<Node> nodes;
        for (auto &child : node->children) {
            vector<Node> childNodes = getQuadtreeNodes(child);
            nodes.insert(nodes.end(), childNodes.begin(), childNodes.end());
        }
        return nodes;
    }
}

vector<vector<vec2>> getNodesVertexes(Quadtree &qt) {
    vector<Node> nodes = getQuadtreeNodes(qt.root);
    // cout << "nodes.size: " << nodes.size() << endl;
    vector<vector<vec2>> positions;
    for (auto &node : nodes) {
        Position(node.origin.x, node.origin.y + node.length),
            positions.push_back({
                vec2(node.origin.x, node.origin.y),
                vec2(node.origin.x + node.length, node.origin.y),
                vec2(node.origin.x + node.length, node.origin.y + node.length),
                vec2(node.origin.x, node.origin.y + node.length),
            });
    }
    return positions;
}

void View::update() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);

    positions.clear();
    for (auto &p : qt.particles) {
        positions.push_back(vec2(p.position.x, p.position.y));
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glUniformMatrix4fv(matProj, 1, GL_FALSE, value_ptr(projection));
    glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(vec2), positions.data());
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS, 0, positions.size());

    // qt vbo
    // cout << "qt root" << qt.root << endl;
    vector<vector<vec2>> positions = getNodesVertexes(qt);
    // vector<vec2> nodes = {
    //     vec2(0, 0),
    //     vec2(qt.length, 0),
    //     vec2(qt.length, qt.length),
    //     vec2(0, qt.length),
    // };
    glBindVertexArray(VAOqt);
    glBindBuffer(GL_ARRAY_BUFFER, VBOqt);
    for (auto &pos : positions) {
        // cout << pos[0].x << " " << pos[0].y << endl;
        // cout << pos[1].x << " " << pos[1].y << endl;
        // cout << pos[2].x << " " << pos[2].y << endl;
        // cout << pos[3].x << " " << pos[3].y << endl;
        glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(vec2), pos.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_LOOP, 0, pos.size());
    }

    // glBindBuffer(GL_ARRAY_BUFFER, VBOqt);
    // glUniformMatrix4fv(matProj, 1, GL_FALSE, value_ptr(projection));
    // glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vec2), positions.data(), GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    // glEnableVertexAttribArray(0);
    // glDrawArrays(GL_LINE_LOOP, 0, nodes.size());

    glfwSwapBuffers(window);
    glfwPollEvents();

    // int fps = 60;
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
}

void View::drawTree() {}

View::View(Quadtree &qt, int windowLength) : qt{qt} {
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(windowLength, windowLength, "N-body simulation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        exit(EXIT_FAILURE);
    }

    shaderProgram = initializeShader();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAOqt);
    glGenBuffers(1, &VBOqt);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    for (auto &p : qt.particles) {
        positions.push_back(vec2(p.position.x, p.position.y));
    }
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec2), positions.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // convertion from world coordinate to screen coordinate
    projection = ortho(0.0f, (float)qt.length, 0.0f, (float)qt.length, -1.0f, 1.0f);
    matProj = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(matProj, 1, GL_FALSE, value_ptr(projection));

    glEnableVertexAttribArray(0);
    glUseProgram(shaderProgram);

    glPointSize(1);
    glLineWidth(0.1);
    glEnable(GL_POINT_SMOOTH);

    // // qt vbo
    // vector<vec2> nodes = {
    //     vec2(0, 0),
    //     vec2(qt.length, 0),
    //     vec2(0, qt.length),
    //     vec2(qt.length, qt.length),
    // };
    // glBindBuffer(GL_ARRAY_BUFFER, VBOqt);
    // glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vec2), nodes.data(), GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
    // glDrawArrays(GL_LINES, 0, 4);
}

View::~View() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}