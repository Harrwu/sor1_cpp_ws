#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

float VERTICES_[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};  

unsigned int INDICES_[] = { 
    0, 1, 3,
    1, 2, 3
};

class Graphics {
private:
    int WINDOW_SIZE_X;
    int WINDOW_SIZE_Y;
    GLFWwindow* window;

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };  

    unsigned int VBO, VAO, EBO;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;

public:
    Graphics(int width, int height);
    ~Graphics() {}

    GLFWwindow* get_window() { return window; }

    void processInput ();
    void framebuffer_size_callback();
    const bool init();
    void run();
};