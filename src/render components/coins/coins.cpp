#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>

#include "./../../shaders/shaders.hpp"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

unsigned int coins_shader;
unsigned int coins_texture;
unsigned int coins_VAO;
std::vector<float> coins_vertices;
std::vector<unsigned int> coins_indices;
float offset_coins = 0.0f;
float prev_time_coins = 0.0f;

float vertices[32] = {
    0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};

void initCoins(){

    createShader("Users/harshavardhan/Documents/CG/Assignment 1/2D Game/src/shaders/vertex.shader",
    "Users/harshavardhan/Documents/CG/Assignment 1/2D Game/src/shaders/fragment.shader",
    &coins_shader);

    glGenVertexArrays(1, &coins_VAO);

    glGenTextures(1, &coins_texture);
    glBindTexture(GL_TEXTURE_2D, coins_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("Users/harshavardhan/Documents/CG/Assignment 1/2D Game/src/textures/coin.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    for(int i = 0 ; i < 32 ; i++){
        coins_vertices.push_back(vertices[i]);
    }

    coins_indices.push_back(0);
    coins_indices.push_back(1);
    coins_indices.push_back(3);
    coins_indices.push_back(1);
    coins_indices.push_back(2);
    coins_indices.push_back(3);

    stbi_image_free(data);

    srand(time(0));

    // print the vertices and indices
    for(int i = 0 ; i < coins_vertices.size() ; i++){
        std::cout << coins_vertices[i] << " ";
    }
    std::cout << std::endl;

    for(int i = 0 ; i < coins_indices.size() ; i++){
        std::cout << coins_indices[i] << " ";
    }
    std::cout << std::endl;

}

void renderCoins(){

    unsigned int VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(coins_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coins_vertices.size(), &coins_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * coins_indices.size(), &coins_indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, coins_texture);

    glUseProgram(coins_shader);

    glUniform1i(glGetUniformLocation(coins_shader, "texture1"), 0);

    glm::mat4 translate = glm::mat4(1.0f);
    unsigned int transformLoc = glGetUniformLocation(coins_shader, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(translate));

    glBindVertexArray(coins_VAO);
    glDrawElements(GL_TRIANGLES, coins_indices.size(), GL_UNSIGNED_INT, 0);

}

void deleteCoins(){

    glDeleteVertexArrays(1, &coins_VAO);
    glDeleteTextures(1, &coins_texture);
    glDeleteProgram(coins_shader);

}
