#include "Mesh.hpp"

Mesh::Mesh(std::vector <Vertex>& p_vertices, std::vector <GLuint>& p_indices, std::vector <Texture>& p_textures) :
    meshVBO(p_vertices), meshEBO(p_indices)
{
    this->textures = p_textures;
	this->vertices = p_vertices;
	this->indices = p_indices;
	
	VAO.Bind();
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAtrrib(meshVBO, meshEBO, 0, 3, GL_FLOAT, sizeof(Vertex), 0);
    VAO.LinkAtrrib(meshVBO, meshEBO, 1, 3, GL_FLOAT, sizeof(Vertex), 3 * sizeof(GLfloat));
    VAO.LinkAtrrib(meshVBO, meshEBO, 2, 2, GL_FLOAT, sizeof(Vertex), 6 * sizeof(GLfloat));
	VAO.LinkAtrrib(meshVBO, meshEBO, 3, 4, GL_FLOAT, sizeof(Vertex), 8 * sizeof(GLfloat));
}



void Mesh::Draw(Shader& shader, Camera& camera)
{
	// Bind shader to be able to access uniforms
	shader.Activate();
	

	//Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;
    
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;

        textures[i].Bind();
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
        textures[i].texUnit(shader, (type + num).c_str(), i);
	}
	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");
	
    VAO.Bind();
	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}


