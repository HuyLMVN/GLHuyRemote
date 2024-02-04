#ifndef EBO_HPP
#define EBO_HPP

#include <glad/gl.h>
#include <vector>
class EBO {
    public:
        GLuint ID;
        EBO(std::vector<GLuint>& indices);
        void Delete();
};

#endif
