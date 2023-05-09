#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include <random>

#include "resourceLoader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, bool* s)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        *s = true;
    else
        *s = false;
}


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    // triangel generation

    float vertices[] = {
        -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
         0.5f,  0.5f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 0.0f
    };

    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vertex buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    // index buffer
    uint32_t indices[] = {
        0, 2, 3,
        0, 1, 2
    };

    uint32_t IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // shader

    // vertex

    std::string vertexShaderSource = readFile("./vertex.shader");
    const char* vertexSrc = vertexShaderSource.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
    }

    // fragment
       
    std::string fragmentShaderSource = readFile("./fragment.shader");
    const char* src = fragmentShaderSource.c_str();
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &src, NULL);
    glCompileShader(fragmentShader);

    int success2;
    char infoLog2[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success2);

    if (!success2) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog2);
        std::cout << infoLog2 << std::endl;
    }

    // program
    uint32_t shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int success3;
    char infoLog3[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success3);
    if (!success3) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog3);
        std::cout << infoLog3 << std::endl;
    }

    int colorLocation = glGetUniformLocation(shaderProgram, "color");
    
    glUseProgram(shaderProgram);

    // texture

    const int width = 1;

    int xWorldLocation = glGetUniformLocation(shaderProgram, "width");
    glUniform1i(xWorldLocation, width);
    
    int radiusSquaredLocation = glGetUniformLocation(shaderProgram, "radiusSquared");
    glUniform1f(radiusSquaredLocation, 0.0001);


    float tex[width][3];

    for (int i = 0; i < width; i++) {
        tex[i][0] = (float)rand() / RAND_MAX;
        tex[i][1] = (float)rand() / RAND_MAX;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_1D, texture);

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, width, 0, GL_RGB, GL_FLOAT, &tex[0][0]);
    glBindTexture(GL_TEXTURE_1D, texture);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        // input
        bool s = false;
        processInput(window, &s);

        // rendering
        if (!s)
        {
            glClearColor(0.0f, 0.45f, 0.5f, 1.0f);
            glUniform4f(colorLocation, 1.0f, 0.45f, 0.0f, 1.0f);
        }
        else
        {
            glClearColor(1.0f, 0.45f, 0.0f, 1.0f);
            glUniform4f(colorLocation, 0.0f, 0.45f, 0.5f, 1.0f);
        }
        glClear(GL_COLOR_BUFFER_BIT);


        for (int i = 0; i < width; i++) {
            tex[i][0] += 0.01*((float)rand() / RAND_MAX -0.5);
            tex[i][1] += 0.01*((float)rand() / RAND_MAX -0.5);
        }
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, width, 0, GL_RGB, GL_FLOAT, &tex[0][0]);


        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}