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

GLuint VBO, VAO;
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

void View::update() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);

    positions.clear();
    for (auto &p : qt.particles) {
        positions.push_back(vec2(p.position.x, p.position.y));
    }

    glUniformMatrix4fv(matProj, 1, GL_FALSE, value_ptr(projection));
    glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(vec2), positions.data());
    glDrawArrays(GL_POINTS, 0, positions.size());

    glfwSwapBuffers(window);
    glfwPollEvents();

    // int fps = 60;
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
}

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

    glPointSize(2);
    glEnable(GL_POINT_SMOOTH);
}

View::~View() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}