#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#include <iostream>
#include <vector>

int prev_zap_bit = 0;
int collision_bit = 0;

int perpZapperCollision(float* character_vertices, std::vector<float>& zapper_vertices);
int diagZapperCollision(float* character_vertices, std::vector<float>& zapper_vertices);

int checkZapperCollision(float* character_vertices, std::vector<float>& zapper_vertices){

    if(zapper_vertices[0] >= -0.88f && zapper_vertices[18] <= -0.77f){
        if(collision_bit == 0){
            if(zapper_vertices[8] == 1.0f){
                if(perpZapperCollision(character_vertices, zapper_vertices)){
                    collision_bit = 1;
                }
            }else{
                if(diagZapperCollision(character_vertices, zapper_vertices)){
                    collision_bit = 1;
                }
            }
        }
    }else{
        collision_bit = 0;
    }

    if(prev_zap_bit == 0){
        if(collision_bit == 1){
            prev_zap_bit = 1;
            return 1;
        }else{
            return 0;
        }
    }else{
        if(collision_bit == 0){
            prev_zap_bit = 0;
        }
        return 0;
    }

}

int coinCollision(float* character_vertices, std::vector<float>& coins_vertices, int index);

void checkCoinCollision(float* character_vertices, std::vector<float>& coins_vertices){

    for(int i = 0 ; i < collision_set.size() ; i++){
        if(collision_set[i] == 0){
            if(coinCollision(character_vertices, coins_vertices, i)){
                collision_set[i] = 1;
            }
        }
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
            if(val < 0.00001f){
                return 1;
            }

        }
    }

    return 0;

}

int coinCollision(float* character_vertices, std::vector<float>& coins_vertices, int index){

    int offset = index * 32;
    if(((coins_vertices[offset + 0] <= -0.77f && coins_vertices[offset + 0] >= -0.88f)
        || (coins_vertices[offset + 16] <= -0.77f && coins_vertices[offset + 16] >= -0.88f))
        && ((coins_vertices[offset + 1] <= character_vertices[1] && coins_vertices[offset + 1] >= character_vertices[3])
        || (coins_vertices[offset + 9] <= character_vertices[1] && coins_vertices[offset + 9] >= character_vertices[3]))){
        return 1;
    }else{
        return 0;
    }


}
