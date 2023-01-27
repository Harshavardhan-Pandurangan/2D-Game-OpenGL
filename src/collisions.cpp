#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#include <iostream>
#include <vector>

int perpZapperCollision(float* character_vertices, std::vector<float>& zapper_vertices);
int diagZapperCollision(float* character_vertices, std::vector<float>& zapper_vertices);

int collision = 0;
int collision_bit = 0;

void checkCollision(float* character_vertices, std::vector<float>& zapper_vertices){

    if(zapper_vertices[0] >= -0.88f && zapper_vertices[18] <= -0.77f){
        if(collision_bit == 0){
            if(zapper_vertices[8] == 1.0f){
                if(perpZapperCollision(character_vertices, zapper_vertices)){
                    std::cout << "Collision detected" << std::endl;
                    collision_bit = 1;
                    collision++;
                }
            }else{
                if(diagZapperCollision(character_vertices, zapper_vertices)){
                    std::cout << "Collision detected" << std::endl;
                    collision_bit = 1;
                    collision++;
                }
            }
        }
    }else{
        collision_bit = 0;
    }

}

int perpZapperCollision(float* character_vertices, std::vector<float>& zapper_vertices){

    if((character_vertices[1] <= zapper_vertices[28] && character_vertices[1] >= zapper_vertices[19]
    || character_vertices[3] <= zapper_vertices[28] && character_vertices[3] >= zapper_vertices[19])){
        return 1;
    }else{
        return 0;
    }

}

int diagZapperCollision(float* character_vertices, std::vector<float>& zapper_vertices){

    for(int i = 0 ; i < 36 ; i+=9){
        for(int j = 0 ; j < 8 ; j+=2){

            float val = abs(sqrt(pow(character_vertices[j] - zapper_vertices[i], 2) + pow(character_vertices[j+1] - zapper_vertices[i+1], 2)))
                        + abs(sqrt(pow(character_vertices[j] - zapper_vertices[(i+9)%36], 2) + pow(character_vertices[j+1] - zapper_vertices[(i+9)%36+1], 2)))
                        - abs(sqrt(pow(zapper_vertices[i] - zapper_vertices[(i+9)%36], 2) + pow(zapper_vertices[i+1] - zapper_vertices[(i+9)%36+1], 2)));
            if(val < 0.01f){
                return 1;
            }

        }
    }

    return 0;

}
