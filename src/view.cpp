#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <cassert>
#include <chrono>
#include <thread>

#include "view.hpp"
#include "quadtree.hpp"

#define G 6.674304

using namespace std;
using namespace glm;

GLuint VBO, VAO, VBOqt, VAOqt;

GLuint shaderProgram;
GLuint projectionMatrixID;
GLFWwindow *window;
mat4 projection;

const char *vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
uniform mat4 projection;
void main() {
    gl_Position = projection*vec4(aPos, 0.0, 1.0);
}
)";

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

void View::clear() { glClear(GL_COLOR_BUFFER_BIT); }

void View::loadTree(Quadtree &qt) {
    vector<vector<vec2>> nodesPositions = getNodesVertexes(qt);
    glBindVertexArray(VAOqt);
    glBindBuffer(GL_ARRAY_BUFFER, VBOqt);
    for (auto &pos : nodesPositions) {
        glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(vec2), pos.data(), GL_STATIC_DRAW);
        glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, value_ptr(projection));
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_LOOP, 0, pos.size());
    }
}

void View::loadParticles(vector<Particle> &particles) {
    vector<vec2> particlePositions;
    for (auto &p : particles) {
        particlePositions.push_back(vec2(p.position.x, p.position.y));
    }

    // bind buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // load projection matrix and particle positions
    glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, value_ptr(projection));
    glBufferSubData(GL_ARRAY_BUFFER, 0, particlePositions.size() * sizeof(vec2), particlePositions.data());
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS, 0, particlePositions.size());
}

void View::render() {
    // glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void initializeVertexBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAOqt);
    glGenBuffers(1, &VBOqt);
}

View::View(Quadtree &qt, int windowLength) {
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
    initializeVertexBuffers();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    vector<vec2> positions;
    for (auto &p : qt.particles) {
        positions.push_back(vec2(p.position.x, p.position.y));
    }
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec2), positions.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    // convertion from world coordinate to screen coordinate
    projection = ortho(0.0f, (float)qt.length, 0.0f, (float)qt.length, -1.0f, 1.0f);
    projectionMatrixID = glGetUniformLocation(shaderProgram, "projection");

    glUseProgram(shaderProgram);
    glPointSize(4);
    glLineWidth(0.1);
    // glEnable(GL_POINT_SMOOTH);
}

View::~View() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAOqt);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBOqt);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}