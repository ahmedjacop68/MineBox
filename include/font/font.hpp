#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

class TextRenderer {
public:
    unsigned int VAO, VBO;
    GLuint textureID;
    GLuint shaderProgram;

    TextRenderer(GLuint shader, GLuint texture) 
        : shaderProgram(shader), textureID(texture) {
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //configure the vertex pos just like last time
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(3);
    }

    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color) {
        glUseProgram(shaderProgram);
        glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glBindVertexArray(VAO);

        float charWidth = 8.0f * scale;
        float charHeight = 8.0f * scale;

        for (char c : text) {
            // Calculate UV coordinates for a 16x16 grid
            float u = (float)(c % 16) / 16.0f;
            float v = (float)(c / 16) / 16.0f;
            float uvSize = 1.0f / 16.0f;

            float vertices[6][4] = {
                { x,             y + charHeight, u,          v },
                { x,             y,              u,          v + uvSize },
                { x + charWidth, y,              u + uvSize, v + uvSize },

                { x,             y + charHeight, u,          v },
                { x + charWidth, y,              u + uvSize, v + uvSize },
                { x + charWidth, y + charHeight, u + uvSize, v }           
            };

            // Update VBO and draw
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            x += charWidth; // Advance to next character position
        }
        glBindVertexArray(3);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};