#pragma once
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <algorithm>

#include "Camera.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "shaderClass.hpp"


class Model {
    public:
        Model(const std::string path);
        Model(const Model& c_Model);
        Model(Model&& m_Model);

        virtual ~Model();
        
        void Draw(Shader& shader, Camera& camera);
        void setPos(Shader& shader, const glm::vec3& sPos);
        std::vector<Texture> loadMaterialTexture(aiMaterial *mat, aiTextureType Type, std::string typeName, const aiScene* scene);
        std::string directory;
    private:
        
        std::vector<Mesh> meshes;
        
        void loadModel(const std::string& path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        static void cleanMesh(Mesh& mesh);
        
};
