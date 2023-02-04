#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "./shaders/shaders.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <map>
#include <string>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include <iostream>

unsigned int hud_shader;
unsigned int hud_VAO;
unsigned int hud_VBO;
unsigned int hud_texture;
unsigned int life_shader;
unsigned int life_VAO;
unsigned int life_texture;

const unsigned int SCR_WIDTH2 = 1200;
const unsigned int SCR_HEIGHT2 = 500;

glm::mat4 projection;

struct Character {
    unsigned int TextureID;
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
};

std::map<GLchar, Character> Characters;

float life_vertices[] = {
    0.97f, 0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.97f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.6f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f, -4.0f, 0.0f,
    0.6f, 0.8f, 0.0f, 1.0f, 0.0f, 0.0f, -4.0f, 1.0f
};

unsigned int life_indices[] = {0, 1, 3, 1, 2, 3};

void RenderText(std::string, float, float, float, glm::vec3);

void initHUD() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    createShader("./../files/shaders/text.vs", "./../files/shaders/text.fs", &hud_shader);

    projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH2), 0.0f, static_cast<float>(SCR_HEIGHT2));

    FT_Library ft;
    FT_Init_FreeType(&ft);
    FT_Face face;
    FT_New_Face(ft, "./../files/render components/plaguard-font/Plaguard-ZVnjx.ttf", 0, &face);

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(unsigned char c = 0 ; c < 128 ; c++){
        FT_Load_Char(face, c, FT_LOAD_RENDER);
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &hud_VAO);
    glGenBuffers(1, &hud_VBO);
    glBindVertexArray(hud_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, hud_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    createShader("./../files/shaders/vertex.shader", "./../files/shaders/fragment.shader", &life_shader);

    glGenVertexArrays(1, &life_VAO);

    glGenTextures(1, &life_texture);
    glBindTexture(GL_TEXTURE_2D, life_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./../files/render components/Lives.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

}

void renderHUD(int lives, short level_bit, int level_){

    glUseProgram(hud_shader);

    if(lives > 0 && level_ < 4){
        std::string dist = ("Dist: " + std::to_string((int)(glfwGetTime() * 4)));
        std::string coins = ("Coins: " + std::to_string(coins_collected));
        std::string level = ("Level: " + std::to_string(level_));

        RenderText(dist, 25.0f, 450.0f, 0.7f, glm::vec3(1.0, 1.0f, 0.8f));
        RenderText(coins, 25.0f, 400.0f, 0.9f, glm::vec3(1.0, 1.0f, 0.8f));
        RenderText(level, 1000.0f, 450.0f, 0.9f, glm::vec3(1.0, 1.0f, 0.8f));
    }

    glUseProgram(life_shader);

    if(life_vertices[22] != 1 - lives){
        life_vertices[22] = 1 - lives;
        life_vertices[30] = 1 - lives;
        life_vertices[16] += 0.074;
        life_vertices[24] += 0.074;
    }

    unsigned int lifeVBO, lifeEBO;
    glGenBuffers(1, &lifeVBO);
    glGenBuffers(1, &lifeEBO);

    glBindVertexArray(life_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, lifeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 32, &life_vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lifeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, &life_indices[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, life_texture);

    glUseProgram(life_shader);

    glUniform1i(glGetUniformLocation(life_shader, "texture1"), 0);

    glm::mat4 translate = glm::mat4(1.0f);
    unsigned int translateLoc = glGetUniformLocation(life_shader, "translate");
    glUniformMatrix4fv(translateLoc, 1, GL_FALSE, glm::value_ptr(translate));

    glBindVertexArray(life_VAO);
    if(level_ < 4){
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    if(lives == 0){
        std::string coins = ("Coins: " + std::to_string(coins_collected));
        RenderText(coins, 500.0f, 100.0f, 1.0f, glm::vec3(1.0, 0.0f, 0.0f));
    }else if(level_ == 4){
        std::string coins = ("Coins: " + std::to_string(coins_collected));
        RenderText(coins, 500.0f, 100.0f, 1.0f, glm::vec3(1.0, 1.0f, 0.0f));
    }

}

void deleteHUD() {

    glDeleteVertexArrays(1, &hud_VAO);
    glDeleteBuffers(1, &hud_VBO);
    glDeleteTextures(1, &hud_texture);
    glDeleteProgram(hud_shader);

    glDeleteVertexArrays(1, &life_VAO);
    glDeleteTextures(1, &life_texture);
    glDeleteProgram(life_shader);

}

void RenderText(std::string text, float x, float y, float scale, glm::vec3 color){

    glUseProgram(hud_shader);
    glUniform3f(glGetUniformLocation(hud_shader, "textColor"), color.x, color.y, color.z);
    glUniformMatrix4fv(glGetUniformLocation(hud_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(hud_VAO);

    std::string text2 = text;
    std::string::const_iterator c;
    for(c = text2.begin(); c != text2.end() ; c++){
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, hud_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }


    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}
