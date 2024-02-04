#include "Texture.hpp"
#include "stb/stb_image.h"
#include <iostream>
Texture::Texture(const char* image, std::string& texType, GLuint slot) {
    
    int widthImg, heightImg, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned short* data = stbi_load_16(image, &widthImg, &heightImg, &nrChannels, 0);
        
    glCreateTextures(GL_TEXTURE_2D, 1, &ID);
    glBindTextureUnit(slot, ID);
    this->unit = slot;
    this->path = image;
    this->type = texType;
    glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if(texType == "specular") {
        glTextureParameteri(ID, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTextureParameteri(ID, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTextureParameteri(ID, GL_TEXTURE_SWIZZLE_A, GL_RED);
    }
    
    if (nrChannels == 1) {
        glTextureStorage2D(ID, 1, GL_RGBA16, widthImg, heightImg);
        glTextureSubImage2D(ID, 0, 0, 0, widthImg, heightImg, GL_RED, GL_UNSIGNED_SHORT, data);
    }
    else if (nrChannels == 4) {
        glTextureStorage2D(ID, 1, GL_RGBA16, widthImg, heightImg);
        glTextureSubImage2D(ID, 0, 0, 0, widthImg, heightImg, GL_RGBA, GL_UNSIGNED_SHORT, data);
    }
    else if (nrChannels == 3) {
        glTextureStorage2D(ID, 1, GL_RGBA16, widthImg, heightImg);
        glTextureSubImage2D(ID, 0, 0, 0, widthImg, heightImg, GL_RGB, GL_UNSIGNED_SHORT, data);
    }
    else if (nrChannels == 2) {
        glTextureStorage2D(ID, 1, GL_RGBA16, widthImg, heightImg);
        glTextureSubImage2D(ID, 0, 0, 0, widthImg, heightImg, GL_RG, GL_UNSIGNED_SHORT, data);
    }
    else {
		throw std::invalid_argument("Automatic Texture type recognition failed");
    }

    glGenerateTextureMipmap(ID);
    stbi_image_free(data);
}
Texture::Texture(const unsigned char* binary, int texW, int texH, std::string& texType, GLuint slot) {
    
    int widthImg, heightImg, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    uint16_t* data = nullptr;
    if (texH == 0) 
        data = stbi_load_16_from_memory(binary, texW, &widthImg, &heightImg, &nrChannels, 0);
    else
        data = stbi_load_16_from_memory(binary, texW * texH, &widthImg, &heightImg, &nrChannels, 0);
    
    glCreateTextures(GL_TEXTURE_2D, 1, &ID);
    glBindTextureUnit(slot, ID);

    this->unit = slot;
    this->type = texType;
    glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameterf(ID, GL_TEXTURE_MAX_ANISOTROPY, 16);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (texType == "specular" || nrChannels == 1){
        glTextureStorage2D(ID, 1, GL_RGBA16, widthImg, heightImg);
        glTextureSubImage2D(ID, 0, 0, 0, widthImg, heightImg, GL_RED, GL_UNSIGNED_SHORT, data);
        glTextureParameteri(ID, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTextureParameteri(ID, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTextureParameteri(ID, GL_TEXTURE_SWIZZLE_A, GL_RED);
    }
    else if (nrChannels == 4) {
        glTextureStorage2D(ID, 1, GL_RGBA16, widthImg, heightImg);
        glTextureSubImage2D(ID, 0, 0, 0, widthImg, heightImg, GL_RGBA, GL_UNSIGNED_SHORT, data);
    }
    else if (nrChannels == 3) {
        glTextureStorage2D(ID, 1, GL_RGBA16, widthImg, heightImg);
        glTextureSubImage2D(ID, 0, 0, 0, widthImg, heightImg, GL_RGB, GL_UNSIGNED_SHORT, data);
    }
    else if (nrChannels == 2) {
        glTextureStorage2D(ID, 1, GL_RGBA16, widthImg, heightImg);
        glTextureSubImage2D(ID, 0, 0, 0, widthImg, heightImg, GL_RG, GL_UNSIGNED_SHORT, data);
    }
    else {
		throw std::invalid_argument("Automatic Texture type recognition failed");
    }

    glGenerateTextureMipmap(ID);
    stbi_image_free(data);
}


void Texture::texUnit(Shader &shader, const char *uniform, GLuint unit) {
    shader.Activate();
    glUniform1i(glGetUniformLocation(shader.ID, uniform), unit);
}

void Texture::Bind() {
    glBindTextureUnit(unit, ID);
}

void Texture::Unbind() {
    glBindTextureUnit(0, 0);
}

void Texture::Delete() {
    glDeleteTextures(1, &ID);
}
