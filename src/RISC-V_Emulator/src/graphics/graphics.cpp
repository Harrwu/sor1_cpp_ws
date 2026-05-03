#include "graphics.h"
#include <iostream>
#include <cmath>

Graphics::Graphics(int width, int height) {
    WINDOW_SIZE_X = width;
    WINDOW_SIZE_Y = height;
}

const bool Graphics::init() {
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFW window init
    window = glfwCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "PONG", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // Vertex Shader
    paddle.vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(paddle.vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(paddle.vertexShader);

    {
        int success;
        char infoLog[512];
        glGetShaderiv(paddle.vertexShader, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(paddle.vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            return false;
        } 
    }

    // Fragment Shader
    paddle.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(paddle.fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(paddle.fragmentShader);

    {
        int success;
        char infoLog[512];
        glGetShaderiv(paddle.fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(paddle.fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            return false;
        }
    }

    // Link shaders
    paddle.shaderProgram = glCreateProgram();
    glAttachShader(paddle.shaderProgram, paddle.vertexShader);
    glAttachShader(paddle.shaderProgram, paddle.fragmentShader);
    glLinkProgram(paddle.shaderProgram);

    {   
        int success;
        char infoLog[512];
        glGetProgramiv(paddle.shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(paddle.shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            return false;
        }
    }    

    // Delete these don't need them already linked to shaderProgram
    glDeleteShader(paddle.vertexShader);
    glDeleteShader(paddle.fragmentShader); 

    // INIT vertex Array Object Vertex buffer object and element buffer object
    glGenVertexArrays(1, &paddle.VAO);
    glGenBuffers(1, &paddle.VBO);
    glGenBuffers(1, &paddle.EBO);

    // bind the vao first then bind and set vertex buffers and then configure vertex attributes
    glBindVertexArray(paddle.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, paddle.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_), VERTICES_, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, paddle.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES_), INDICES_, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    return true;
}

void Graphics::build_circle(const float x, const float y){
    float theata = 360.0f / (float)CIR_POINTS_;
    float angle = theata;
    for (int i = 0; i < CIR_POINTS_ * 3; i += 3){
        float radians = 
        CIRCLE_VERT_ [i] = std::cos(theata) * CIR_RADIUS_;
        CIRCLE_VERT_ [i + 1] = std::sin(theata) * CIR_RADIUS_;
        CIRCLE_VERT_ [i + 2] = 0.0f;
    }
}

void Graphics::run() {
    playerOne.yPos = 0.25;
    playerTwo.yPos = 0.25;

    while (!glfwWindowShouldClose(window)) {
        processInput();
        glClearColor(0.f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(paddle.shaderProgram);
        glBindVertexArray(paddle.VAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void Graphics::processInput(){
    float speed = 0.01f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        playerOne.yIncrecement = speed;
        change_vert_Quad(VERTICES_, 0, playerOne);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        playerOne.yIncrecement = -speed;
        change_vert_Quad(VERTICES_, 0, playerOne);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        playerTwo.yIncrecement = speed;
        change_vert_Quad(VERTICES_, 12, playerTwo);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        playerTwo.yIncrecement = -speed;
        change_vert_Quad(VERTICES_, 12, playerTwo);
    }
}

void Graphics::change_vert_Quad(float* vertices, int startIndex, Player &player) {
    float movement = player.yIncrecement;
    
    int endIndex = startIndex + 12;

    for (int i = startIndex + 1; i < endIndex; i += 3) {
        float nextY = vertices[i] + movement;
        if (nextY > 1.0f || nextY < -1.0f) return;
    }

    for (int i = startIndex + 1; i < endIndex; i += 3) {
        vertices[i] += movement;
    }

    glBindBuffer(GL_ARRAY_BUFFER, paddle.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VERTICES_), vertices);
}

int main() {
    Graphics graph(800, 600);
    if (graph.init()) {
        graph.run();
    }
    return 0;
}