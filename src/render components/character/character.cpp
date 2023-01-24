#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "./../../shaders/shaders.hpp"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif
#include "./../../controls.cpp"

unsigned int character_shader;
unsigned int character_texture;
unsigned int character_VAO;
float curr_elevation = 0.0f;
float gravity = 0.015f;
int step_tracker = 0;

void initCharacter(){
    createShader("/Users/harshavardhan/Documents/CG/Assignment 1/2D Game/src/shaders/vertexshader.txt",
    "/Users/harshavardhan/Documents/CG/Assignment 1/2D Game/src/shaders/fragmentshader.txt",
    &character_shader);

    float vertices[] = {
        -0.77f,  -0.58f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        -0.77f, -0.88f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        -0.88f, -0.88f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        -0.88f,  -0.58f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, EBO;
    glGenVertexArrays(1, &character_VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(character_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void renderCharacter(GLFWwindow *window){

    int press = processInput(window);

    glUseProgram(character_shader);

    if(press == 1){
        if(curr_elevation < 1.3f)
            curr_elevation += 0.01f;
        gravity = 0.01f;
        step_tracker = 0;
    }
    else if(curr_elevation > 0.0f){
        gravity += 0.0003f;
        (curr_elevation > gravity) ? curr_elevation -= gravity : curr_elevation = 0.0f;
        step_tracker = 0;
    }else{
        step_tracker = (step_tracker + 1) % 80;
    }

    glBindTexture(GL_TEXTURE_2D, character_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data;

    if(curr_elevation > 0.0f){
        data = stbi_load("/Users/harshavardhan/Documents/CG/Assignment 1/2D Game/src/render components/character/Char flying.png", &width, &height, &nrChannels, 0);
    }else if(step_tracker < 20){
        data = stbi_load("/Users/harshavardhan/Documents/CG/Assignment 1/2D Game/src/render components/character/Char step 1.png", &width, &height, &nrChannels, 0);
    }else if(step_tracker < 40){
        data = stbi_load("/Users/harshavardhan/Documents/CG/Assignment 1/2D Game/src/render components/character/Char step 2.png", &width, &height, &nrChannels, 0);
    }else if(step_tracker < 60){
        data = stbi_load("/Users/harshavardhan/Documents/CG/Assignment 1/2D Game/src/render components/character/Char step 3.png", &width, &height, &nrChannels, 0);
    }else if(step_tracker < 80){
        data = stbi_load("/Users/harshavardhan/Documents/CG/Assignment 1/2D Game/src/render components/character/Char step 4.png", &width, &height, &nrChannels, 0);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glUniform1i(glGetUniformLocation(character_shader, "texture1"), 0);

    glm::mat4 translate = glm::mat4(1.0f);
    translate = glm::translate(translate, glm::vec3(0.0f, curr_elevation, 0.0f));

    unsigned int translateLoc = glGetUniformLocation(character_shader, "translate");
    glUniformMatrix4fv(translateLoc, 1, GL_FALSE, glm::value_ptr(translate));

    glBindVertexArray(character_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void deleteCharacter(){
    glDeleteVertexArrays(1, &character_VAO);
    glDeleteProgram(character_shader);
    glDeleteTextures(1, &character_texture);
}
