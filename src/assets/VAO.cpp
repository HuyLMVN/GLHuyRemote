#include "VAO.hpp"

VAO::VAO() {
    glCreateVertexArrays(1, &ID);
}

void VAO::LinkAtrrib(VBO &VBO, EBO &EBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, GLintptr offset) {
    glVertexArrayAttribBinding(ID, layout, 0);
    glVertexArrayAttribFormat(ID, layout, numComponents, type, GL_FALSE, offset);
    glEnableVertexArrayAttrib(ID, layout);
    glVertexArrayVertexBuffer(ID, 0, VBO.ID, 0, stride);
    glVertexArrayElementBuffer(ID, EBO.ID);
}

void VAO::Bind() {
    glBindVertexArray(ID);
}

void VAO::Unbind() {
    glBindVertexArray(0);
}

void VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
}
