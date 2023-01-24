#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>

#include <iostream>

#include "setup.cpp"
#include "./render components/background/background.cpp"
#include "./render components/character/character.cpp"
#include "./render components/lazers/lazers.cpp"

int main()
{
    GLFWwindow* window = setupGLFW();
    if(window == (GLFWwindow*)-1){
        return -1;
    }

    initBackground();
    initCharacter();
    initLazers();

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderBackground();
        renderCharacter(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    deleteBackground();
    deleteCharacter();

    glfwTerminate();
    return 0;
}
