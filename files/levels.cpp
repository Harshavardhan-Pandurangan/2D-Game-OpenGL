#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "./shaders/shaders.hpp"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include <iostream>

unsigned int end_shader;
unsigned int end_vao;
unsigned int end_vbo, end_ebo;
unsigned int end_texture;
unsigned int end_texture1;

void checkLevel(short *level, short *level_bit){

    if(glfwGetTime() > 20){
        *level += 1;
        *level_bit = 1;
        glfwSetTime(0);
    }

}

void increaseLevel(short *level, short *level_bit){

    if(*level_bit == 1){
        *level_bit = 0;
    }

    return;

}

void initWasted(){

    createShader("./../files/shaders/vertex.shader",
    "./../files/shaders/fragment.shader",
    &end_shader);

    float vertices[] = {
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0,
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0,
        -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0
    };

    unsigned int indices[] = {0, 1, 3, 1, 2, 3};

    glGenVertexArrays(1, &end_vao);
    glGenBuffers(1, &end_vbo);
    glGenBuffers(1, &end_ebo);

    glBindVertexArray(end_vao);

    glBindBuffer(GL_ARRAY_BUFFER, end_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, end_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenTextures(1, &end_texture);
    glBindTexture(GL_TEXTURE_2D, end_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./../files/render components/wasted.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glGenTextures(1, &end_texture1);
    glBindTexture(GL_TEXTURE_2D, end_texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width1, height1, nrChannels1;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data1 = stbi_load("./../files/render components/done.png", &width1, &height1, &nrChannels1, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data1);

}

void renderWasted(){

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, end_texture);
    glUseProgram(end_shader);

    glm::mat4 translate = glm::mat4(1.0f);
    unsigned int translateLoc = glGetUniformLocation(end_shader, "translate");
    glUniformMatrix4fv(translateLoc, 1, GL_FALSE, glm::value_ptr(translate));

    glUniform1i(glGetUniformLocation(end_shader, "texture1"), 0);

    glBindVertexArray(end_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void renderDone(){

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, end_texture1);
    glUseProgram(end_shader);

    glm::mat4 translate = glm::mat4(1.0f);
    unsigned int translateLoc = glGetUniformLocation(end_shader, "translate");
    glUniformMatrix4fv(translateLoc, 1, GL_FALSE, glm::value_ptr(translate));

    glUniform1i(glGetUniformLocation(end_shader, "texture1"), 0);

    glBindVertexArray(end_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}
