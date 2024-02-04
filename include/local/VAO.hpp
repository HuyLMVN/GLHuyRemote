#ifndef VAO_HPP
#define VAO_HPP

#include <glad/gl.h>
#include <VBO.hpp>
#include <EBO.hpp>

class VAO {

    public:
        GLuint ID;
        VAO();
        void LinkAtrrib(VBO &VBO, EBO &EBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, GLintptr offset);

        void Bind();
        void Unbind();
        void Delete();
};

#endif
