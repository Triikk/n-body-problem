#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include <chrono>
#include <thread>

#include "view.hpp"

using namespace std;
using namespace glm;

void View::update() {}

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

// layout(location = 1) in mat4 model;
// model*vec4

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
)";

GLuint compileShader(GLenum type, const char* source) {
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
void setupBuffers(vector<Particle> particles) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    vector<vec2> positions;
    for (auto& p : particles) {
        positions.push_back(vec2(p.position.x, p.position.y));
    }
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec2), particles.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

#include <iostream>

View::View(int width, int height, vector<Particle> particles) {
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    GLFWwindow* window = glfwCreateWindow(width, height, "N-body simulation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        exit(EXIT_FAILURE);
    }

    mat4 projection = ortho(0.0f, float(width), 0.0f, float(height), -1.0f, 1.0f);

    GLuint shaderProgram = initializeShader();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    vector<vec2> positions;
    for (auto& p : particles) {
        positions.push_back(vec2(p.position.x, p.position.y));
    }
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec2), positions.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPointSize(10);
    glEnable(GL_POINT_SMOOTH);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        // glBindVertexArray(VAO);

        for (auto& pos : positions) {
            pos.x = (rand() % 100) / 50.0 - 1;
            pos.y = (rand() % 100) / 50.0 - 1;
            cout << pos.x << "," << pos.y << endl;
        }

        glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(vec2), positions.data());
        glDrawArrays(GL_POINTS, 0, positions.size());

        glfwSwapBuffers(window);
        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

View::~View() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteProgram(shaderProgram);

    // glfwDestroyWindow(window);
    glfwTerminate();
}