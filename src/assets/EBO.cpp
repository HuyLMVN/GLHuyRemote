#include "EBO.hpp"

EBO::EBO(std::vector<GLuint>& indices) {
    glCreateBuffers(1, &ID);
    glNamedBufferData(ID, indices.size() * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);
}

void EBO::Delete() {
    glDeleteBuffers(1, &ID);
}
