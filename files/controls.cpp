#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

int processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        return 1;
    return 0;
}
