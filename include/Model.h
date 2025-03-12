#pragma once

#include <glad/glad.h>
#include <glm.hpp>
#include <vector>
#include <string>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

class Model {
public:
   Model(const std::string& path);
   void Draw(Shader& shader);

private:
   std::vector<Mesh> meshes;
   std::string directory;

   void loadModel(const std::string& path);
   void processNode(aiNode* node, const aiScene* scene);
   Mesh processMesh(aiMesh* mesh, const aiScene* scene);
   std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene);
};
