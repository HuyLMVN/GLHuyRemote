#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/gl.h>
#include <stb/stb_image.h>
#include "shaderClass.hpp"
class Texture {
    public:
        GLuint ID;
        std::string type;
        std::string path;
        GLuint unit;
        Texture(const char* image, std::string& texType, GLuint slot);
        Texture(const unsigned char* binary, int texW, int texH, std::string& texType, GLuint slot);
        void texUnit(Shader& shader, const char* uniform, GLuint unit);
        void Bind();
        void Unbind();
        void Delete();

};

#endif
