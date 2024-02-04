#include "VBO.hpp"

VBO::VBO(std::vector<Vertex>& vertices){ 
    glCreateBuffers(1, &ID);
    glNamedBufferData(ID, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
}

void VBO::Delete() {
    glDeleteBuffers(1, &ID);
}
