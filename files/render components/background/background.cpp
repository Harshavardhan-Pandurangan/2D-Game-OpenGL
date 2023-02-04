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

unsigned int background_shader;
unsigned int background_texture;
unsigned int background_VAO;
float offset_background = 0.0f;
float prev_time_background = 0.0f;
float speed_control_background = 0.5f / 0.9f;

void initBackground(){

    createShader("./../files/shaders/vertex.shader",
    "./../files/shaders/fragment.shader",
    &background_shader);

    float vertices[] = {
        (5.0f/3.0f),  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        (5.0f/3.0f), -1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 1.0f,

         1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        (1.0f/3.0f), -1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        (1.0f/3.0f),  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f,

        (1.0/3.0f),  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        (1.0/3.0f), -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        (-1.0/3.0f), -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
        (-1.0/3.0f),  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

        (-1.0/3.0f),  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        (-1.0/3.0f), -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
        4, 5, 7,
        5, 6, 7,
        8, 9, 11,
        9, 10, 11,
        12, 13, 15,
        13, 14, 15
    };

    unsigned int VBO, EBO;
    glGenVertexArrays(1, &background_VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(background_VAO);

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

    glGenTextures(1, &background_texture);
    glBindTexture(GL_TEXTURE_2D, background_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./../files/render components/background/background 2.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

}

void renderBackground(int level_bit, int level){

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, background_texture);
    glUseProgram(background_shader);

    if(level_bit == 1){
        offset_background = 0.0f;
        prev_time_background = 0.0f;
        speed_control_background = ((speed_control_background * 0.9f) + 0.2f) / 0.9f;
        if(level == 3){
            glBindTexture(GL_TEXTURE_2D, background_texture);
            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true);
            unsigned char *data = stbi_load("./../files/render components/background/FinalRound.png", &width, &height, &nrChannels, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(data);
        }
    }

    if(offset_background > -0.665f){
        float curr_time_background = (float)glfwGetTime() * speed_control_background;
        offset_background = offset_background + prev_time_background - curr_time_background;
        prev_time_background = curr_time_background;
    }else{
        offset_background = 0.0f;
    }

    glm::mat4 translate = glm::mat4(1.0f);
    translate = glm::translate(translate, glm::vec3(offset_background, 0.0f, 0.0f));

    unsigned int translateLoc = glGetUniformLocation(background_shader, "translate");
    glUniformMatrix4fv(translateLoc, 1, GL_FALSE, glm::value_ptr(translate));

    glUniform1i(glGetUniformLocation(background_shader, "texture1"), 0);

    glBindVertexArray(background_VAO);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

}

void deleteBackground(){

    glDeleteVertexArrays(1, &background_VAO);
    glDeleteProgram(background_shader);
    glDeleteTextures(1, &background_texture);

}
