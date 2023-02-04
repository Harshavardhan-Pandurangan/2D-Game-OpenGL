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
float character_vertices[8] = {
    -0.77f,  -0.58f,
    -0.77f, -0.88f,
    -0.88f, -0.88f,
    -0.88f,  -0.58f
};
float curr_elevation = 0.0f;
float curr_velocity = 0.0f;
float acceleration = 0.0f;
float gravity = 0.0f;
int step_tracker = 0;
short image_control = 0;
float prev_time_character = 0.0f;
float acceleration_control = 0.07f;

void initCharacter(){

    createShader("./../files/shaders/vertex.shader",
    "./../files/shaders/fragment.shader",
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

void renderCharacter(GLFWwindow *window, int collision_bit, short level_bit, int lives){

    if(level_bit == 1){
        curr_elevation = 0.0f;
        curr_velocity = 0.0f;
        acceleration = 0.0f;
        gravity = 0.0f;
        step_tracker = 0;
        image_control = 0;
        prev_time_character = 0.0f;
        acceleration_control = acceleration_control + 0.02f;
    }

    int press = processInput(window);
    if(lives == 0){
        press = 0;
    }

    glUseProgram(character_shader);

    acceleration = (glfwGetTime() - prev_time_character) * acceleration_control;
    prev_time_character = glfwGetTime();
    gravity = acceleration / 2.0f;

    if(curr_elevation == 0.0f){
        step_tracker = (step_tracker + 1) % 68;
        if(press == 1){
            curr_elevation = 0.001f;
            curr_velocity = 0.001f;
        }
    }else if(curr_elevation < 0.0f){
        if(press == 1){
            curr_velocity += acceleration - gravity;
            if(curr_velocity > 0.03f){
                curr_velocity = 0.03f;
            }
            curr_elevation = 0.0f;
        }else if(curr_velocity < 0.0f){
            curr_velocity = 0.0f;
            curr_elevation = 0.0f;
        }else{
            step_tracker = (step_tracker + 1) % 68;
            curr_elevation = 0.0f;
        }
    }else{
        if(press == 1){
            curr_velocity += acceleration - gravity;
            if(curr_velocity > 0.03f){
                curr_velocity = 0.03f;
            }
        }else{
            curr_velocity -= gravity;
            if(curr_velocity < -0.03f){
                curr_velocity = -0.03f;
            }
        }
        curr_elevation += curr_velocity;
        if(curr_elevation >= 1.5f){
            curr_velocity = 0.0f;
            curr_elevation = 1.5f;
        }
        gravity = 0.0004f;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, character_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data;

    if(collision_bit == 1){
        data = stbi_load("./../files/render components/character/PicFinal5.png", &width, &height, &nrChannels, 0);
    }else if(curr_elevation > 0.0f){
        data = stbi_load("./../files/render components/character/FinalFly.png", &width, &height, &nrChannels, 0);
    }else if(step_tracker < 17){
        data = stbi_load("./../files/render components/character/PicFinal1.png", &width, &height, &nrChannels, 0);
    }else if(step_tracker < 34){
        data = stbi_load("./../files/render components/character/PicFinal2.png", &width, &height, &nrChannels, 0);
    }else if(step_tracker < 51){
        data = stbi_load("./../files/render components/character/PicFinal3.png", &width, &height, &nrChannels, 0);
    }else if(step_tracker < 68){
        data = stbi_load("./../files/render components/character/PicFinal4.png", &width, &height, &nrChannels, 0);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glUniform1i(glGetUniformLocation(character_shader, "texture1"), 0);

    glm::mat4 translate = glm::mat4(1.0f);
    translate = glm::translate(translate, glm::vec3(0.0f, curr_elevation, 0.0f));

    int fly;
    unsigned int glowLoc = glGetUniformLocation(character_shader, "fly");
    if(press == 1){
        fly = 1;
        glUniform1i(glowLoc, fly);
    }else{
        fly = 0;
        glUniform1i(glowLoc, fly);
    }

    character_vertices[1] = -0.58f + curr_elevation;
    character_vertices[3] = -0.88f + curr_elevation;
    character_vertices[5] = -0.88f + curr_elevation;
    character_vertices[7] = -0.58f + curr_elevation;

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
