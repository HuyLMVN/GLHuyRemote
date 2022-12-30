#include "ModelLoad.hpp"
#include <assimp/postprocess.h>

Model::Model(const std::string path) {
    loadModel(path);
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,  aiProcess_SortByPType | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_GenUVCoords | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_ImproveCacheLocality );   
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
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));	
        
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    // walk through each of the mesh's vertices
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
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
            }
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.s = mesh->mTextureCoords[0][i].x;
                vec.t = mesh->mTextureCoords[0][i].y;
                vertex.texUV = vec;
                
            }
            else {
                vertex.texUV = glm::vec2(0.0f, 0.0f);
            }
            vertices.push_back(vertex);
        }

            
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);        
        }
        // process materials
        ;
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "diffuse", scene);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Texture> roughnessMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE_ROUGHNESS, "specular", scene);
        textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
        
        
    return Mesh(vertices, indices, textures);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
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
            if(!skip) { 
                if(typeName == "diffuse") {
                    if(((tPtr = scene->GetEmbeddedTexture(str.C_Str())) != nullptr)) {
                        const unsigned char* texData= static_cast<unsigned char*>(static_cast<void*>(tPtr->pcData));
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
                        const unsigned char* texData= static_cast<unsigned char*>(static_cast<void*>(tPtr->pcData));
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

void Model::setPos(Shader& shader, const glm::vec3& sPos) {
    glm::mat4 uPos = glm::mat4(1.0f);
    uPos = glm::translate(sPos);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "uPos"), 1, GL_FALSE, glm::value_ptr(uPos));
}
void Model::cleanMesh(Mesh& mesh) {
    mesh.VAO.Delete();
    mesh.meshVBO.Delete();
    mesh.meshEBO.Delete();
    for(auto& tex : mesh.textures) {
        tex.Delete();
    }
}

Model::Model(const Model& c_Model) = default;
Model::Model(Model&& m_Model) = default;

Model::~Model() {
    std::for_each(meshes.begin(), meshes.end(), this->cleanMesh);
}




