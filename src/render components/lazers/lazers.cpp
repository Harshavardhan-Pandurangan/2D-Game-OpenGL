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

unsigned int lazer_shader;
unsigned int lazer_texture;
unsigned int lazer_VAO;

void initLazers(){
    std::cout << "Initializing lazers" << std::endl;
}
