#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>

#include <iostream>

#include "setup.cpp"
#include "./render components/background/background.cpp"
#include "./render components/character/character.cpp"
#include "./render components/zappers/zappers.cpp"
#include "./render components/coins/coins.cpp"
#include "./collisions.cpp"

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

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderBackground();
        renderCharacter(window);
        renderZappers();
        renderCoins();

        checkCollision(character_vertices, zapper_vertices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    deleteBackground();
    deleteCharacter();
    deleteZappers();
    deleteCoins();

    glfwTerminate();
    return 0;
}
