#include "ModelLoad.hpp"

Model::Model(const std::string path) {
    loadModel(path);
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_SortByPType | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_GenUVCoords | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_ImproveCacheLocality);   
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::Draw(Shader &shader, Camera &camera) {
    for(unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader, camera);
    }
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // Process nodes' meshes
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));	
        
    }
    // Recursive node proceesing for children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    // Walk through each of the mesh's vertices and retrive vertex coords, normals and texture coords
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        //aiColor4D diffuse = {0.0f, 0.0f, 0.0f, 0.0f};
        //if(AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
          //  vertex.color = {diffuse.r, diffuse.g, diffuse.b, diffuse.a};
        //}
        // Retrieve normals
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // Retrieve texture coordinates
        if(mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.s = mesh->mTextureCoords[0][i].x;
            vec.t = mesh->mTextureCoords[0][i].y;
            vertex.texUV = vec;
            
        }
        else {
            vertex.texUV = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

        
    // Retrieve indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);        
    }
    // Process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "diffuse", scene);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    
    std::vector<Texture> roughnessMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE_ROUGHNESS, "specular", scene);
    textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
        
    return Mesh(vertices, indices, textures);
}


std::vector<Texture> Model::loadMaterialTexture(aiMaterial *mat, aiTextureType Type, std::string typeName, const aiScene* scene) {
    std::vector<Texture> textures;
    std::vector<std::string> loadedTexPath;
    std::vector<Texture> loadedTex;
    for(unsigned int i = 0; i < mat->GetTextureCount(Type); i++)
    {
        aiString str;
        const aiTexture* tPtr = nullptr;
        
        mat->GetTexture(Type, i, &str);
        
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        
        for(unsigned int j = 0; j < loadedTex.size(); j++)
        {
            if((loadedTex[j].path == str.data))
            {
                textures.push_back(loadedTex[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        // Put diffuse texture in slot 0, specular textures in slot 1
        if(!skip) { 
            if(typeName == "diffuse") {
                if(((tPtr = scene->GetEmbeddedTexture(str.C_Str())) != nullptr)) {
                    
                    const unsigned char* texData= reinterpret_cast<unsigned char*>(tPtr->pcData);
                    Texture texture(texData, tPtr->mWidth, tPtr->mHeight, typeName, 0);
                    loadedTex.push_back(texture);
                    textures.push_back(texture);
                } 
                else {
                    Texture texture((directory + '/' + str.data).c_str(), typeName, 0);                   
                    loadedTex.push_back(texture);
                    textures.push_back(texture);
                }
            }
            else if(typeName == "specular"){ 
                if(((tPtr = scene->GetEmbeddedTexture(str.C_Str())) != nullptr)) {
                    const unsigned char* texData= reinterpret_cast<unsigned char*>(tPtr->pcData);
                    Texture texture(texData, tPtr->mWidth, tPtr->mHeight, typeName, 1);
                    loadedTex.push_back(texture);
                    textures.push_back(texture);
                } 
                else {
                    Texture texture((directory + '/' + str.data).c_str(), typeName, 1);                   
                    loadedTex.push_back(texture);
                    textures.push_back(texture);
                }    
            }
        }
    }
    return textures;
}
// Set models' Posiiton
void Model::setPos(Shader& shader, const glm::vec3& sPos) {
    glm::mat4 uPos = glm::mat4(1.0f);
    uPos = glm::translate(sPos);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "uPos"), 1, GL_FALSE, glm::value_ptr(uPos));
}

// Delete VAO, VBO and EBO once done.
void Model::cleanMesh(Mesh& mesh) {
    mesh.VAO.Delete();
    mesh.meshVBO.Delete();
    mesh.meshEBO.Delete();
    for(auto& texture : mesh.textures) {
        texture.Delete();
    }
}

Model::Model(const Model& c_Model) = default;
Model::Model(Model&& m_Model) = default;
// Custom destructor
Model::~Model() {
    std::for_each(meshes.begin(), meshes.end(), this->cleanMesh);
}




