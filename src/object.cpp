#include <glad/glad.h> 
#include <iostream>
#include <object/object.hpp>

void InitVertex(unsigned int VBO, unsigned int VAO, unsigned int EBO, float* Vertices, unsigned int* indices, size_t versize, size_t insize)
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, versize, Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, insize, indices, GL_STATIC_DRAW);

}