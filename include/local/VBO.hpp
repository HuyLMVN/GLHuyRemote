#ifndef VBO_HPP
#define VBO_HPP
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texUV;
    glm::vec4 color;
};
class VBO {
    public:
        GLuint ID;
    
        VBO(std::vector<Vertex>& vertices);
        
        void Delete();
    
};


#endif
