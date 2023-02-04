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
std::vector<float> coins_set_coords;
float prev_time_coins = 0.0f;
int prev_gen_sec_coin = -2;
int coins_collected = 0;
int coin_phase = 0;
float speed_control_coins = 0.5f / 0.9f;
float regen_c = 1.0f;

float verticesCoin[32] = {
    0.02f, 0.58f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.02f, 0.51f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.02f, 0.51f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.02f, 0.58f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

std::vector<int> collision_set;

float verticesCoins[416];

void prepVertices(){

    int k = 0;

    for(int i = -4 ; i < 5 ; i++){
        if(i % 2 == 0){

            verticesCoins[k * 32] = verticesCoin[0] + i * 0.04f;
            verticesCoins[k * 32 + 1] = verticesCoin[1];
            verticesCoins[k * 32 + 6] = verticesCoin[6];
            verticesCoins[k * 32 + 7] = verticesCoin[7];

            verticesCoins[k * 32 + 8] = verticesCoin[8] + i * 0.04f;
            verticesCoins[k * 32 + 9] = verticesCoin[9];
            verticesCoins[k * 32 + 14] = verticesCoin[14];
            verticesCoins[k * 32 + 15] = verticesCoin[15];

            verticesCoins[k * 32 + 16] = verticesCoin[16] + i * 0.04f;
            verticesCoins[k * 32 + 17] = verticesCoin[17];
            verticesCoins[k * 32 + 22] = verticesCoin[22];
            verticesCoins[k * 32 + 23] = verticesCoin[23];

            verticesCoins[k * 32 + 24] = verticesCoin[24] + i * 0.04f;
            verticesCoins[k * 32 + 25] = verticesCoin[25];
            verticesCoins[k * 32 + 30] = verticesCoin[30];
            verticesCoins[k * 32 + 31] = verticesCoin[31];

            k++;

        }else{

            verticesCoins[k * 32] = verticesCoin[0] + i * 0.04f;
            verticesCoins[k * 32 + 1] = verticesCoin[1] + 0.12f;
            verticesCoins[k * 32 + 6] = verticesCoin[6];
            verticesCoins[k * 32 + 7] = verticesCoin[7];

            verticesCoins[k * 32 + 8] = verticesCoin[8] + i * 0.04f;
            verticesCoins[k * 32 + 9] = verticesCoin[9] + 0.12f;
            verticesCoins[k * 32 + 14] = verticesCoin[14];
            verticesCoins[k * 32 + 15] = verticesCoin[15];

            verticesCoins[k * 32 + 16] = verticesCoin[16] + i * 0.04f;
            verticesCoins[k * 32 + 17] = verticesCoin[17] + 0.12f;
            verticesCoins[k * 32 + 22] = verticesCoin[22];
            verticesCoins[k * 32 + 23] = verticesCoin[23];

            verticesCoins[k * 32 + 24] = verticesCoin[24] + i * 0.04f;
            verticesCoins[k * 32 + 25] = verticesCoin[25] + 0.12f;
            verticesCoins[k * 32 + 30] = verticesCoin[30];
            verticesCoins[k * 32 + 31] = verticesCoin[31];

            k++;

            verticesCoins[k * 32] = verticesCoin[0] + i * 0.04f;
            verticesCoins[k * 32 + 1] = verticesCoin[1] - 0.12f;
            verticesCoins[k * 32 + 6] = verticesCoin[6];
            verticesCoins[k * 32 + 7] = verticesCoin[7];

            verticesCoins[k * 32 + 8] = verticesCoin[8] + i * 0.04f;
            verticesCoins[k * 32 + 9] = verticesCoin[9] - 0.12f;
            verticesCoins[k * 32 + 14] = verticesCoin[14];
            verticesCoins[k * 32 + 15] = verticesCoin[15];

            verticesCoins[k * 32 + 16] = verticesCoin[16] + i * 0.04f;
            verticesCoins[k * 32 + 17] = verticesCoin[17] - 0.12f;
            verticesCoins[k * 32 + 22] = verticesCoin[22];
            verticesCoins[k * 32 + 23] = verticesCoin[23];

            verticesCoins[k * 32 + 24] = verticesCoin[24] + i * 0.04f;
            verticesCoins[k * 32 + 25] = verticesCoin[25] - 0.12f;
            verticesCoins[k * 32 + 30] = verticesCoin[30];
            verticesCoins[k * 32 + 31] = verticesCoin[31];

            k++;

        }
    }

    return;

}

void initCoins(){

    prepVertices();

    createShader("./../files/shaders/vertex.shader",
    "./../files/shaders/fragment.shader",
    &coins_shader);

    glGenVertexArrays(1, &coins_VAO);

    glGenTextures(1, &coins_texture);
    glBindTexture(GL_TEXTURE_2D, coins_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./../files/render components/coins/Coin.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    srand(time(0));

}

void renderCoins(short zapper_pos, int* permit, short level_bit){

    if(level_bit == 1){
        coins_vertices.clear();
        coins_set_coords.clear();
        coins_indices.clear();
        collision_set.clear();
        prev_gen_sec_coin = -2;
        prev_time_coins = 0.0f;
        speed_control_coins = ((speed_control_coins * 0.9f) + 0.2f) / 0.9f;
        regen_c -= 0.3f;
    }

    int coin_pos = rand() % 2;
    if(zapper_pos == 0){
        if(coin_pos == 0){
            coin_pos = -1;
        }
    }else if(zapper_pos == 1){
        if(coin_pos == 1){
            coin_pos = -1;
        }
    }
    coin_pos--;

    unsigned int type_indices[6] = {0, 1, 3, 1, 2, 3};

    if(prev_gen_sec_coin < (int)glfwGetTime()-regen_c && *permit == 1){
        for(int i = 0 ; i < 416 ; i++){
            coins_vertices.push_back(verticesCoins[i] + (i % 8 == 0 ? 1.2f : 0.0f) + (i % 8 == 1 ? coin_pos * 0.6f : 0.0f));
        }
        coins_set_coords.push_back(coins_vertices[16]);
        coins_set_coords.push_back(coins_vertices[399]);
        for(int i = 0 ; i < 13 ; i++){
            collision_set.push_back(0);
        }

        if(coins_indices.size() < 312){
            for(int i = 0 ; i < 13 ; i++){
                for(int j = 0 ; j < 6 ; j++){
                    coins_indices.push_back(type_indices[j] + coins_indices.size() / 6 * 4);
                }
            }
        }

        prev_gen_sec_coin = (int)glfwGetTime();
        *permit = 0;
    }

    if(coins_vertices[384] < -1.2f){
        coins_vertices.erase(coins_vertices.begin(), coins_vertices.begin() + 416);
        coins_set_coords.erase(coins_set_coords.begin());
        coins_set_coords.erase(coins_set_coords.begin());
        collision_set.erase(collision_set.begin(), collision_set.begin() + 13);
    }

    for(int i = 0 ; i < collision_set.size() ; i++){
        if(collision_set[i] == 1){
            collision_set[i] = 2;
            for(int j = 0 ; j < 32 ; j++){
                if(j % 8 == 0){
                    continue;
                }
                coins_vertices[i * 32 + j] = 0.0f;
            }
            coins_collected++;
        }
    }

    if((((int)(glfwGetTime()*10.0f)%10)/(float)10 > coin_phase * 0.25f) && (((int)(glfwGetTime()*10.0f)%10)/(float)10 < (coin_phase + 1) * 0.25f)){
        for(int i = 0 ; i < coins_vertices.size()-31 ; i+=32){
            if(collision_set[i/32] != 0){
                continue;
            }

            if(coin_phase == 0){
                coins_vertices[i] -= 0.005f;
                coins_vertices[i + 8] -= 0.005f;
                coins_vertices[i + 16] -= -0.005f;
                coins_vertices[i + 24] -= -0.005f;
            }else if(coin_phase == 1){
                coins_vertices[i] -= 0.008f;
                coins_vertices[i + 8] -= 0.008f;
                coins_vertices[i + 16] -= -0.008f;
                coins_vertices[i + 24] -= -0.008f;
            }else if(coin_phase == 2){
                coins_vertices[i] += 0.008f;
                coins_vertices[i + 8] += 0.008f;
                coins_vertices[i + 16] += -0.008f;
                coins_vertices[i + 24] += -0.008f;
            }else if(coin_phase == 3){
                coins_vertices[i] += 0.005f;
                coins_vertices[i + 8] += 0.005f;
                coins_vertices[i + 16] += -0.005f;
                coins_vertices[i + 24] += -0.005f;
            }
        }
        coin_phase = (coin_phase + 1) % 4;
    }

    float curr_time_coins = glfwGetTime() * speed_control_coins;
    if(curr_time_coins < prev_time_coins){
        prev_time_coins = 0.003f;
    }

    for(int i = 0 ; i < coins_vertices.size(); i+=8){
        coins_vertices[i] -= curr_time_coins - prev_time_coins;
    }
    coins_set_coords[0] -= curr_time_coins - prev_time_coins;
    coins_set_coords[1] -= curr_time_coins - prev_time_coins;

    prev_time_coins = curr_time_coins;


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

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, coins_texture);

    glUseProgram(coins_shader);

    glUniform1i(glGetUniformLocation(coins_shader, "texture1"), 0);

    glm::mat4 translate = glm::mat4(1.0f);
    unsigned int translateLoc = glGetUniformLocation(coins_shader, "translate");
    glUniformMatrix4fv(translateLoc, 1, GL_FALSE, glm::value_ptr(translate));

    glBindVertexArray(coins_VAO);
    glDrawElements(GL_TRIANGLES, coins_indices.size(), GL_UNSIGNED_INT, 0);

}

void deleteCoins(){

    glDeleteVertexArrays(1, &coins_VAO);
    glDeleteTextures(1, &coins_texture);
    glDeleteProgram(coins_shader);

}
