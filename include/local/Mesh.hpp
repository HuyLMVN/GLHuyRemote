#pragma once
#include <string>

#include"VAO.hpp"
#include"EBO.hpp"
#include"Camera.hpp"
#include"Texture.hpp"

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	// Store VAO in public so it can be used in the Draw function
	VAO VAO;

	// Initializes the mesh
	Mesh(std::vector <Vertex>& p_vertices, std::vector <GLuint>& p_indices, std::vector <Texture>& p_textures);

	// Draws the mesh
	void Draw(Shader& shader, Camera& camera);
    VBO meshVBO;
    EBO meshEBO;
};
