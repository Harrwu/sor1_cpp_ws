#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdint>

#define PI_ 3.14159265358979323846;

const int CIR_POINTS_ = 12;
const float CIR_RADIUS_ = 0.2f;

float CIRCLE_VERT_[CIR_POINTS_ * 3];

struct Player{
    float yPos = -0.25;
    float yIncrecement = 0.05f;
};

struct objectIDS{
    unsigned int VBO, VAO, EBO;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;
};

float VERTICES_[] = {
    -0.9f, 0.9f, 0.0f,
    -0.9f, 0.5f, 0.0f,
    -0.85f, 0.9f, 0.0f,
    -0.85f, 0.5f, 0.0f,
    0.9f, 0.9f, 0.0f,
    0.9f, 0.5f, 0.0f,
    0.85f, 0.9f, 0.0f,
    0.85f, 0.5f, 0.0f
};  

unsigned int INDICES_[] = { 
    0, 1, 2,
    1, 2, 3,
    4, 5, 6,
    5, 6, 7
};

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n" // Orange
    "}\n\0";

class Graphics {
private:
    int WINDOW_SIZE_X;
    int WINDOW_SIZE_Y;
    GLFWwindow* window;

    Player playerOne, playerTwo;
    objectIDS paddle;

public:
    Graphics(int width, int height);
    ~Graphics() {}

    GLFWwindow* get_window() { return window; }
    void change_vert_Quad(float* vertices, int startIndex, Player &player);
    void build_circle(const float x, const float y);

    void processInput ();
    void framebuffer_size_callback();
    const bool init();
    void run();
};