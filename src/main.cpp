#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>

#include <iostream>

#include "./../files/setup.cpp"
#include "./../files/render components/background/background.cpp"
#include "./../files/render components/character/character.cpp"
#include "./../files/render components/zappers/zappers.cpp"
#include "./../files/render components/coins/coins.cpp"
#include "./../files/collisions.cpp"
#include "./../files/hud.cpp"
#include "./../files/levels.cpp"

short lives = 5;
short level = 1;
short level_bit = 0;

int main()
{
    GLFWwindow* window = setupGLFW();
    if(window == (GLFWwindow*)-1){
        return -1;
    }

    initBackground();
    initCharacter();
    initZappers();
    initCoins();
    initHUD();
    initWasted();

    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.0f, 0.0f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if(lives > 0 && level < 4){
            checkLevel(&level, &level_bit);
        }

        renderBackground(level_bit, level);
        renderZappers(level_bit);
        renderCharacter(window, collision_bit, level_bit, lives);
        renderCoins(zapper_pos, &permit, level_bit);
        if(level_bit == 0 && lives > 0 && level < 4){
            lives -= checkZapperCollision(character_vertices, zapper_vertices);
            checkCoinCollision(character_vertices, coins_vertices);
        }

        renderHUD(lives, level_bit, level);

        if(lives < 1){
            renderWasted();
            renderHUD(lives, level_bit, level);
        }else if(level == 4){
            renderDone();
            renderHUD(lives, level_bit, level);
        }

        if(level_bit == 1 && lives > 0){
            increaseLevel(&level, &level_bit);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    deleteBackground();
    deleteCharacter();
    deleteZappers();
    deleteCoins();
    deleteHUD();

    glfwTerminate();
    return 0;
}
